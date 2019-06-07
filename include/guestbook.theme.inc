<?php
/**
 * @file
 * The implementation of the theme functions.
 */

/**
 * Generates messages html markup.
 *
 * @return
 *   Html markup.
 */
function guestbook_html() {
  global $user;

  // Variable $mes is fully generated html markup.
  $mes = NULL;

  // Get messages data array from database.
  $variables = guestbook_read_db('guestbook_drupal', 'id', 'DESC', '10');

  foreach ($variables as $value) {
    // Get message creation date.
    $date = format_date($value['3'], $type = 'medium', $format = '', $timezone = NULL, $langcode = NULL);
    // Get user name.
    $name = guestbook_name_by_id($value['1']);
    // Generate html markup for date and name.
    $data = '<div><em>'.$date.'  '.$name.'</em></div>';
    // Get message and generate html markup for it.
    $message = '<div><b>'.check_plain($value['4']).'</b></div>';

    // If user is logged and current session id = stored id in database
    // then generate html markup for edit button and delete button.
    if ($user->uid) {
      if ($value['2'] == $user->sid) {
        $params = drupal_get_query_parameters();
        $message .= l(t('edit'), 'guestbook-page/' . $value['0'] . '/edit', array('attributes' => array('class' => array('guestbook-button')), 'query' => $params))
        . l(t('delete'), 'guestbook-page/' . $value['0'] . '/delete', array('attributes' => array('class' => array('guestbook-button')), 'query' => $params));
      }
    }
    $mes .= '<div class = "guestbook-messages">' . $data . $message . '</div>';
  }

  $mes .= theme('pager');
  return '<div class = "guestbook-messages-all">' . $mes . '</div>';
}
