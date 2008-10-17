To download and install from a tarball or zip file:

 Download one of the following site files:

    * http://socialmediaclassroom.com/repo/hgwebdir.cgi/smc/archive/tip.zip
    * http://socialmediaclassroom.com/repo/hgwebdir.cgi/smc/archive/tip.tar.gz

Navigate to your web directory, untar or unzip the site files

 

And, optionally, you may also import a copy of our database from here:

    *  http://socialmediaclassroom.com/smcdb/smcbeta.gz

Run the following command:

shell> mysqladmin create db_name

(substitute your chosen databse name for db_name above)

Then, run:

shell> gunzip < db_name.gz | mysql db_name
Now simply edit sites/default/settings.php to reflect the database name you chose above, and the password, also set "$base_url" to the base url of your site 9with no trailing slash). Save these changes, and visit the site in your web browser.

You can login with psuedo username and password "admin/admin" and change these once you login.

Lastly, visit "yourdomain.com/yourdrupalsite/devel/cache/clear" to clear the cache, and you are done, and ready to configure the site to your heart's content!