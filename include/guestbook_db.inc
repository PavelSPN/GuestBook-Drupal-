<?php

/**
* @file
* Implementation of database access functions.
*/

/**
 * Reading data from the guestbook database.
 *
 * @return
 *   A data array containing complete information about each database entry.
 */
function guestbook_read_db($table = 'guestbook_drupal', $order = 'id',
  $direction = 'DESC', $limit = '10') {
  $rows = array();
  $query = db_select($table, 't')
    ->extend('PagerDefault')
    ->fields('t', array('id', 'uid', 'sid', 'date', 'message'))
    ->orderBy($order, $direction)
    ->limit($limit)
    ->execute();
  while ($value = $query->fetchAssoc()) {
    $rows[] = array(
      'id' => $value['id'],
      'uid' => $value['uid'],
      'sid' => $value['sid'],
      'date' => $value['date'],
      'message' => check_plain($value['message']),
      'user_name' => guestbook_name_by_id($value['uid']),
      'formatted_date' => format_date($value['date'], $type = 'medium',
        $format = '', $timezone = NULL, $langcode = NULL),
    );
  }

  return $rows;
}

/**
 * Save or edit message.
 *
 * Data from hook form. Contains $form['info_by_id'] = array('id', 'uid', 'sid', 'date', 'message').
 * id - message id; uid - user id; sid - session id; date - message creation date; message - message text.
 *
 * @param $form_state
 */
function guestbook_save_edit_db($form_state) {
  global $user;

  // If there is an array of data ($form['info_by_id']['id']), then edit the message
  // otherwise - save as a new message.
  if (isset ($form_state['values']['info_by_id']['id'])) {
    db_update('guestbook_drupal')
      ->fields(array('message' => $form_state['values']['message']))
      ->condition('id', $form_state['values']['info_by_id']['id'])
      ->execute();
  }
  else {
    db_insert('guestbook_drupal')
      ->fields(array(
        'uid' => $user->uid,
        'sid' => $user->sid,
        'date' => guestbook_time(),
        'message' => $form_state['values']['message'],
        )
      )
      ->execute();
  }
}

/**
 * Get user name by user id.
 *
 * @param $uid
 *   Integer specifying the user id.
 * @return
 *   Return user name.
 */
function guestbook_name_by_id($uid) {
  if ($uid) {
    $query = db_select('users', 't')
      ->fields('t', array('name'))
      ->condition('t.uid', $uid)
      ->execute();
    $value = $query->fetchAssoc();
    $name = $value['name'];
  }
  else {
    $name = 'User does not exist';
  }

  return $name;
}

/**
 * Delete message.
 *
 * @param $value
 *   contains array('id', 'uid', 'sid', 'date', 'message').
 *   id - message id; uid - user id; sid - session id; date - message
 *   creation date; message - message text.
 */
function guestbook_message_delete($value) {
  global $user;

  if ($user->uid && $user->sid == $value['sid']) {
    db_delete('guestbook_drupal')
      ->condition('id', $value['id'])
      ->execute();
    drupal_set_message(t('Message deleted!'));
    $params = drupal_get_query_parameters();
    drupal_goto('guestbook-page', array('query' => $params));
  }
  else {
    drupal_goto('guestbook-page');
  }
}
