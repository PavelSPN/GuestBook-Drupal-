<?php

/**
* @file
* Contains db callbacks and functions for guestbook module.
*/

/**
 * Returns data from the guestbook database.
 *
 * @return array
 *   A data array containing complete information about each database entry.
 */
function guestbook_read_db($table = 'guestbook_drupal', $order = 'id', $direction = 'DESC', $limit = '10') {
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
      'formatted_date' => format_date($value['date']),
    );
  }

  return $rows;
}

/**
 * Saves or edits message.
 *
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function guestbook_save_edit_db($form_state) {
  global $user;

  $table = 'guestbook_drupal';
  $record = array(
    'uid' => $user->uid,
    'sid' => $user->sid,
    'date' => guestbook_time(),
    'message' => $form_state['values']['message'],
    'id' => !empty ($form_state['values']['info_by_id']['id']) ? $form_state['values']['info_by_id']['id'] : NULL,
  );

  // If exists 'id', then edit the message, otherwise - save as a new message.
  if ($record['id']) {
    drupal_write_record($table, $record, 'id');
  }
  else {
    drupal_write_record($table, $record);
  }
}

/**
 * Returns user name by user id.
 *
 * @param $uid
 *   Integer specifying the user id.
 *
 * @return string
 *   Returns user name or 'User does not exist'.
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
    $name = t('User does not exist');
  }

  return $name;
}

/**
 * Deletes message.
 *
 * @param $id
 *   $id contains message id.
 */
function guestbook_message_delete($id) {
  db_delete('guestbook_drupal')
    ->condition('id', $id)
    ->execute();
  drupal_set_message(t('Message deleted!'));
  $params = drupal_get_query_parameters();
  drupal_goto('guestbook-page', array('query' => $params));
}

/**
 * Gets info by message id.
 *
 * @param $mid
 *   Integer specifying the message id.
 *
 * @return array
 *   A fully-loaded array of message info ('id', 'uid', 'sid', 'date', 'message').
 *   - id: message id;
 *   - uid: user id;
 *   - sid: session id;
 *   - date: message creation date;
 *   - message: message text.
 */
function guestbook_info_by_mid($mid) {
  $query = db_select('guestbook_drupal', 't')
    ->fields('t', array('id', 'uid', 'sid', 'date', 'message'))
    ->condition('t.id', $mid)
    ->execute();
  $value = $query->fetchAssoc();

  return $value;
}
