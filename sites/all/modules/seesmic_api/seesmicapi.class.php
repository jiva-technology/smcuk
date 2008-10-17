<?php
// $Id: seesmicapi.class.php,v 1.1.2.1 2008/07/03 11:53:43 cyberpunk Exp $ 
/**
 * @file
 * Seesmic API implementation
 *
 * @author Sergey Bozhko (eDrupal.fr)
 */


/**
 * @todo Implement follwing calls:
 *   GET 	/users/username/replies 	get replies for username
 *   GET 	/users/username/follows 	get people username follows
 *   GET 	/users/username/follows/videos 	get videos for the people username follows
 *   GET 	/videos 	retrieve videos for the public timeline
 */

/**
 * JSON encoding/decoding utility
 * if there are no json_decode and json_encode functions, defining and implementing them
 */
if (!function_exists('json_encode')) {
  require 'json.class.php';
  function json_encode($value) {
    $json = new Services_JSON();
    return $json->encode($value);
  }
  function json_decode($json) {
    $json = new Services_JSON();
    return $json->decode($json);
  }
}

/**
 * Seesmic API implementation
 *
 */
class SeesmicAPI {

  // Seesmic endpoint
  const SESSMIC_ENDPOINT  = 'http://api.seesmic.com/';
  // User agent, used for cURL
  const USER_AGENT        = 'Drupal Seesmic API';
  // Application identifier, used for session namespace
  const APP_IDENTIFIER    = 'drupal_seesmic_api_module';

  // Error codes and explanations
  private $_errorCodes = array(
    400 => 'Bad Request: Your request is not valid - message included in body',
    401 => 'Not Authorized: SID parameter is either expired or not valid',
    403 => 'Forbidden: Your request is valid, we just are not going to fulfill it',
    404 => 'Not Found: The resource requested does not exist',
    406 => 'Not Acceptable: Your requested format is not one that is supported',
    410 => 'Gone: The item has been removed from Seesmic.  If you cache items, please update and respect this request from the Seesmic User.',
    500 => 'Internal Server Error: Something broke on our end - please let us know about it',
    503 => 'Service Unavailable: We are updating something on our end, please come back later',

    'authentication_failed' => 'There was an error authenticating you. Please check login and password.'
  );

  // List of actions, that SeesmicAPI can perform
  private $_requestActions = array(
    'login'       => 'login/auth',
    'user'        => 'users/%s.json',
    'user_videos' => 'users/%s/videos.json'
  );

  // Used for storing RAW response
  private $_responseRaw;
  // Used for storing response headers
  private $_responseHeaders;
  // Used for storing parsed as PHP StdClass Object from JSON response
  private $_responseParsed;
  // User name
  private $_userName;
  // User password
  private $_password;
  // String that contains the request to seesmic server
  private $_requestString;
  // Placeholder for error explanation
  private $_errorString;
  // Used for storing action, that is being performed
  private $_operation;
  // cURL engine variable
  private $_curlEngine;

  /**
   * Constructor
   *
   * @param string $operation
   */
  public function __construct($operation) {
    // Initializing action
    $this->_operation   = $operation;
    // Initializing cURL engine
    $this->_curlEngine  = curl_init();
    // Setting up cURL options
    curl_setopt($this->_curlEngine, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt($this->_curlEngine, CURLOPT_USERAGENT,      self::USER_AGENT);
    curl_setopt($this->_curlEngine, CURLOPT_HEADER,         0);
    curl_setopt($this->_curlEngine, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($this->_curlEngine, CURLOPT_HTTP_VERSION,   CURL_HTTP_VERSION_1_1);
    curl_setopt($this->_curlEngine, CURLOPT_CONNECTTIMEOUT, 10);
    curl_setopt($this->_curlEngine, CURLOPT_TIMEOUT,        55);

    // Initializing session storage
    $_SESSION[self::APP_IDENTIFIER]         = array();
    $_SESSION[self::APP_IDENTIFIER]['auth'] = array();
  }

  /**
   * Desctructor
   *
   */
  public function __destruct() {
    // Destroying cURL engine
    curl_close($this->_curlEngine);
    $this->_curlEngine = null;
  }

  /**
   * Main function, that run proccesing of request. Returns true if success.
   *
   * @param array $arguments
   *   Arguments for the action
   * @return bool
   */
  public function process($arguments) {
    switch ($this->_operation) {
      // Login action
      case 'login'        : return $this->authorize(g(0, $arguments), g(1, $arguments)); break;
      // Logout action
      case 'logout'       : return $this->logout();                                      break;
      // User profile action
      case 'user'         : return $this->getProfile(g(0, $arguments));                  break;
      // User videos action
      case 'user_videos'  : return $this->getUserVideos(g(0, $arguments));               break;
    }
  }

  /**
   * Retrives user profile for given user name
   *
   * @param string $user_name
   * @return bool
   */
  public function getProfile($user_name) {
    // Composing request string
    $this->_requestString = sprintf($this->_requestActions['user'], $user_name);
    // Executing API call
    return $this->_execute();
  }

  /**
   * Retrives user videos for given user name
   *
   * @param string $user_name
   * @return bool
   */
  public function getUserVideos($user_name) {
    // Composing request string
    $this->_requestString = sprintf($this->_requestActions['user_videos'], $user_name);
    // Executing API call
    return $this->_execute();
  }

  /**
   * Performs user authentication
   *
   * @param string $user_name
   * @param string $password
   * @return bool
   */
  public function authorize($user_name, $password) {
    $this->_userName = $user_name;
    $this->_password = $password;
    // Generating auth string
    $auth_string = $this->makeAuthString();
    // Composing request string
    $this->_requestString = $this->_requestActions['login'] .'?username='. $this->_userName .'&password='. $auth_string;
    if (!$this->_execute()) {
      return false;
    }

    // Checking if user successfully logged in
    if ($this->_responseParsed->login->success == 'false') {
      // if no, getting an error
      $this->_errorString = $this->_errorCodes[$this->_responseParsed->login->reason];
      return false;
    }
    // Storing user information in session
    $this->_setSessionParams();
    return true;
  }

  /**
   * Performs user logout
   *
   */
  public function logout() {
    // Remove user stuff from session
    unset($_SESSION[self::APP_IDENTIFIER]['auth']);
  }

  /**
   * Analyzes error code and assigns error string. Returns true, if any error occured
   *
   * @param mixed $code
   * @return bool
   */
  public function isError($code) {
    $this->_errorString = '';
    // If any error
    if (isset($this->_errorCodes[$code])) {
      // Assign error string
      $this->_errorString = t($this->_errorCodes[$code]);
      return true;
    }
    // If no errors
    return false;
  }

  /**
   * Storing user params in session
   *
   */
  private function _setSessionParams() {
    // Sid - deprecated
    $_SESSION[self::APP_IDENTIFIER]['auth']['sid']        = $this->_responseParsed->login->sid;
    // Session ID
    $_SESSION[self::APP_IDENTIFIER]['auth']['session_id'] = $this->_responseParsed->login->session_id;
  }

  /**
   * Performs an API call
   *
   */
  private function _makeCall() {
    // Setting an URL
    curl_setopt($this->_curlEngine, CURLOPT_URL, self::SESSMIC_ENDPOINT . $this->_requestString);
    // Getting response
    $this->_responseRaw     = curl_exec($this->_curlEngine);
    // Getting headers
    $this->_responseHeaders = curl_getinfo($this->_curlEngine);
  }

  /**
   * Converts JSON to PHP StdClass object
   *
   */
  private function _parseResponse() {
    // Converts JSON to PHP StdClass object
    $this->_responseParsed = json_decode($this->_responseRaw);
  }

  /**
   * Returns result
   *
   * @return object
   */
  public function getResult() {
    return $this->_responseParsed;
  }

  /**
   * Composes Auth string for user login
   *
   * @return string
   */
  private function makeAuthString() {
    return md5($this->_userName . $this->_password);
  }

  /**
   * Performs call, error analysis, and response parsing
   *
   * @return bool
   */
  private function _execute() {
    // Execute API call
    $this->_makeCall();
    // Analyze error
    if ($this->isError($this->_responseHeaders['http_code'])) {
      return false;
    }
    // Parse response
    $this->_parseResponse();
    return true;
  }

  /**
   * Returns an error string
   *
   * @return string
   */
  public function getError() {
    return $this->_errorString;
  }
}