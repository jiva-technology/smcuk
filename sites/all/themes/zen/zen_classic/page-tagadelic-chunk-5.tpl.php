<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language ?>" xml:lang="<?php print $language ?>">

<head>
<title><?php print $head_title; ?></title>
<?php print $head; ?>
<?php print $styles; ?>
  <!--[if IE]>
    <link rel="stylesheet" href="<?php print $base_path . $directory ?>/ie.css" type="text/css">
  <?php if ($subtheme_directory && file_exists($subtheme_directory .'/ie.css')): ?>
      <link rel="stylesheet" href="<?php print $base_path . $subtheme_directory ?>/ie.css" type="text/css">
  <?php endif; ?>
  <![endif]-->
  <?php print $scripts; ?>
</head>

<?php /* different classes allow for separate theming of the home page */ ?>

<body class="<?php print $body_classes; ?>">
  <div id="page">
    <div id="header">
<div id="skip-nav"><a href="#content"><?php print t('Skip to Main Content'); ?></a></div>

      <div id="logo-title">

<?php print $search_box; ?>
<?php if (!empty($logo)): ?>
          <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" />
          </a>
<?php endif; ?>

        <div id="name-and-slogan">

        <?php if (!empty($site_name)): ?>
          <div id='site-name'><strong>
            <a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>" rel="home">
	<?php print $site_name; ?>
            </a>
          </strong></div>
	    <?php endif; ?>

	    <?php if (!empty($site_slogan)): ?>
          <div id='site-slogan'>
            <?php print $site_slogan; ?>
          </div>
	  <?php endif; ?>

        </div> <!-- /name-and-slogan -->

      </div> <!-- /logo-title -->

      <div id="navigation" class="menu<?php if ($primary_links) { print " withprimary"; } if ($secondary_links) { print " withsecondary"; } ?> ">
        <?php if (!empty($primary_links)): ?>
          <div id="primary" class="clear-block">
	<?php print theme('links', $primary_links); ?>
          </div>
	  <?php endif; ?>

	  <?php if (!empty($secondary_links)): ?>
          <div id="secondary" class="clear-block">
	  <?php print theme('links', $secondary_links); ?>
          </div>
	  <?php endif; ?>
      </div> <!-- /navigation -->
	<div class="smc_toolbar" id="socialbookmarktoolbar">
		<a href="<?php print url("node/add/userlink");?>" id="toolbaraddsocialbookmark">Create</a> | 
		<a href="<?php print url("tagadelic/chunk/5");?>" id="toolbarsocialbookmarktagsall">All Tags</a> | 
		<a href="<?php print url('logout');?>">Logout</a>
		<?php if (!empty($tabs)): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
	</div>

      <?php if (!empty($header)): ?>
        <div id="header-region">
      <?php print $header; ?>
        </div> <!-- /header-region -->
	<?php endif; ?>

    </div> <!-- /header -->

    <div id="container" class="clear-block">

    <?php if (!empty($sidebar_left)): ?>
        <div id="sidebar-left" class="column sidebar">
    <?php print $sidebar_left; ?>
        </div> <!-- /sidebar-left -->
	<?php endif; ?>

      <div id="main" class="column"><div id="squeeze" class="clear-block">
      <?php if (!empty($mission)): ?>
      <div id="mission"><?php print $mission; ?></div>
      <?php endif; ?>
      <?php if (!empty($content_top)):?>
      <div id="content-top"><?php print $content_top; ?></div>
      <?php endif; ?>
        <div id="content">
	<?php if (!empty($title)): ?>
	<h2 class="socialbookmarktitle"><?php print $title; ?></h2>
	<?php endif; ?>
	<?php endif; ?>
	<?php print $help; ?>
	<?php print $messages; ?>
	<?php print $content; ?>
	<?php if (!empty($feed_icons)): ?>
	<div class="feed-icons"><?php print $feed_icons; ?></div>
	<?php endif; ?>
        </div> <!-- /content -->
        <?php if (!empty($content_bottom)): ?>
	<div id="content-bottom"><?php print $content_bottom; ?></div>
        <?php endif; ?>
      </div></div> <!-- /squeeze /main -->

      <?php if (!empty($sidebar_right)): ?>
        <div id="sidebar-right" class="column sidebar">
      <?php print $sidebar_right; ?>
        </div> <!-- /sidebar-right -->
	<?php endif; ?>

    </div> <!-- /container -->

    <div id="footer-wrapper">
      <div id="footer">
    <?php print $footer_message; ?>
      </div> <!-- /footer -->
    </div> <!-- /footer-wrapper -->

      <?php if ($closure_region): ?>
      <div id="closure-blocks"><?php print $closure_region; ?></div>
      <?php endif; ?>

      <?php print $closure; ?>

  </div> <!-- /page -->

</body>
</html>