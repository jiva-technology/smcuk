<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language ?>" xml:lang="<?php print $language ?>">
<head>
<title><?php print $head_title ?></title>
<?php print $head ?><?php print $styles ?><?php print $scripts ?>
<script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
<?php if (theme_get_setting('litejazz_width')) { ?>
<style type="text/css">
      	#page {
			width : <?php print theme_get_setting('litejazz_fixedwidth') ?>px;
		}
      </style>
<?php } else { ?>
<style type="text/css">
      	#page {
      width: 95%;
		}
      </style>
<?php }  ?>
<?php if (theme_get_setting('litejazz_iepngfix')) { ?>
<!--[if lte IE 6]>
<script type="text/javascript"> 
    $(document).ready(function(){ 
        $(document).pngFix(); 
    }); 
</script> 
<![endif]-->
<?php } ?>
</head>
<body>
<div id="page">
  <div id="masthead">
    <div id="header" class="clear-block">
      <div class="header-right">
        <div class="header-left"> <?php print $search_box; ?>
          <div id="logo-title">
            <?php if ($logo): ?>
            <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" /> </a>
            <?php endif; ?>
          </div>
          <!-- /logo-title -->
          <div id="name-and-slogan">
            <?php if ($site_name): ?>
            <h1 id='site-name'> <a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>"> <?php print $site_name; ?> </a> </h1>
            <?php endif; ?>
            <?php if ($site_slogan): ?>
            <div id='site-slogan'> <?php print $site_slogan; ?> </div>
            <?php endif; ?>
          </div>
          <!-- /name-and-slogan -->
          <?php if ($header): ?>
          <?php print $header; ?>
          <?php endif; ?>
        </div>
        <!-- /header-left -->
      </div>
      <!-- /header-right -->
    </div>
    <!-- /header -->
  </div>
  <div id="navigation" class="menu <?php if ($primary_links) { print "withprimary"; } if ($secondary_links) { print " withsecondary"; } ?> ">
    <?php if ($primary_links): ?>
    <div id="primary" class="clear-block"> <?php print theme('menu_links', $primary_links); ?> </div>
    <?php endif; ?>
    <?php if ($secondary_links): ?>
    <div id="secondary" class="clear-block"> <?php print theme('menu_links', $secondary_links); ?> </div>
    <?php endif; ?>
  </div>
  <!-- /navigation -->
  <?php
         $section1count = 0;
         $user1count = 0;
         $user2count = 0;
         $user3count = 0;

         if ($user1)
         {
            $section1count++;
            $user1count++;
         }

         if ($user2)
         {
            $section1count++;
            $user2count++;
         }
         
         if ($user3)
         {
            $section1count++;
            $user3count++;
         }
      ?>
  <?php if ($section1count): ?>
  <?php $section1width = 'width' . floor(99 / $section1count); ?>
  <?php $block2div = ($user1count and ($user2count or $user3count)) ? " divider" : ""; ?>
  <?php $block3div = ($user3count and ($user1count or $user2count)) ? " divider" : ""; ?>
  <div class="clr" id="section1">
    <table class="sections" cellspacing="0" cellpadding="0">
      <tr valign="top">
        <?php if ($user1): ?>
        <td class="section <?php echo $section1width ?>"><?php print $user1; ?> </td>
        <?php endif; ?>
        <?php if ($user2): ?>
        <td class="section <?php echo $section1width . $block2div; ?>"><?php print $user2; ?> </td>
        <?php endif; ?>
        <?php if ($user3): ?>
        <td class="section <?php echo $section1width . $block3div; ?>"><?php print $user3; ?> </td>
        <?php endif; ?>
      </tr>
    </table>
  </div>
  <!-- /section1 -->
  <?php endif; ?>
  <div id="middlecontainer">
    <table border="0" cellpadding="0" cellspacing="0" id="content">
      <tr>
        <?php if ($sidebar_left) { ?>
        <td id="sidebar-left"><?php print $sidebar_left ?> </td>
        <?php } ?>
        <td valign="top"><?php if ($mission) { ?>
          <div id="mission"><?php print $mission ?></div>
          <?php } ?>
          <div id="main">
            <?php if (theme_get_setting('litejazz_breadcrumb')): ?>
            <?php if ($breadcrumb): ?>
            <div id="breadcrumb"> <?php print $breadcrumb; ?> </div>
            <?php endif; ?>
            <?php endif; ?>
            <?php if ($content_top):?>
            <div id="content-top"><?php print $content_top; ?></div>
            <?php endif; ?>
            <h1 class="title"><?php print $title ?></h1>
            <div class="tabs"><?php print $tabs ?></div>
            <?php print $help ?> <?php print $messages ?> <?php print $content; ?> <?php print $feed_icons; ?>
            <?php if ($content_bottom): ?>
            <div id="content-bottom"><?php print $content_bottom; ?></div>
            <?php endif; ?>
          </div></td>
        <?php if ($sidebar_right) { ?>
        <td id="sidebar-right"><?php print $sidebar_right ?> </td>
        <?php } ?>
      </tr>
    </table>
  </div>
  <?php
         $section2count = 0;
         $user4count = 0;
         $user5count = 0;
         $user6count = 0;

         if ($user4)
         {
            $section2count++;
            $user4count++;
         }

         if ($user5)
         {
            $section2count++;
            $user5count++;
         }
         
         if ($user6)
         {
            $section2count++;
            $user6count++;
         }
      ?>
  <?php if ($section2count): ?>
  <?php $section2width = 'width' . floor(99 / $section2count); ?>
  <?php $block2div = ($user4count and ($user5count or $user6count)) ? " divider" : ""; ?>
  <?php $block3div = ($user6count and ($user4count or $user5count)) ? " divider" : ""; ?>
  <div class="clr" id="section2">
    <table class="sections" cellspacing="0" cellpadding="0">
      <tr valign="top">
        <?php if ($user4): ?>
        <td class="section <?php echo $section2width ?>"><?php print $user4; ?> </td>
        <?php endif; ?>
        <?php if ($user5): ?>
        <td class="section <?php echo $section2width . $block2div; ?>"><?php print $user5; ?> </td>
        <?php endif; ?>
        <?php if ($user6): ?>
        <td class="section <?php echo $section2width . $block3div; ?>"><?php print $user6; ?> </td>
        <?php endif; ?>
      </tr>
    </table>
  </div>
  <!-- /section2 -->
  <?php endif; ?>
  <div id="footer"> <?php print $footer_message ?> <br />
    <a href="http://www.roopletheme.com" title="RoopleTheme!"><img src="<?php print base_path() . path_to_theme() . "/roopletheme.png"; ?>" alt="RoopleTheme"/></a> </div>
  <div id="footer-wrapper" class="clear-block">
    <div class="footer-right">
      <div class="footer-left"> </div>
      <!-- /footer-left -->
    </div>
    <!-- /footer-right -->
  </div>
  <!-- /footer-wrapper -->
  <?php print $closure ?> </div>
</body>
</html>
