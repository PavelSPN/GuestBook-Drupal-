<?php

/**
 * @file
 * Theme preprocess functions.
 */

/**
 * Implements hook_preprocess_hook() for guestbook_message_template.
 */
function guestbook_preprocess_guestbook_message_template(&$variables){
  // Add variables to the array.
  global $user;
  $variables['user_object'] = $user;
}
