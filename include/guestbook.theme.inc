<?php

/**
 * @file
 * Contains theme preprocess functions.
 */

/**
 * Implements hook_preprocess_hook().
 *
 * Adds global variable user,
 * 'delete' and 'edit' links to message array.
 */
function guestbook_preprocess_guestbook_message_template(&$variables){
  // Add global variable user.
  global $user;
  $variables['user_object'] = $user;
  // Add 'delete' and 'edit' links to message array.
  $params = drupal_get_query_parameters();
  $i = 0;
  $options = array(
    'attributes' => array(
      'class' => array('guestbook-button'),
    ),
    'query' => $params,
  );
  foreach ($variables['body'] as $value){
    $links = l(t('edit'), 'guestbook-page/' . $value['id'] . '/edit', $options);
    $links .= l(t('delete'), 'guestbook-page/' . $value['id'] . '/delete', $options);
    $variables['body'][$i] += array(
      'links' => $links,
    );
    $i++;
  }
}
