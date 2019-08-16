<?php

/**
 * @file
 * Contains form callback for module guestbook.
 */

/**
 * Guestbook page form.
 *
 * @param $value
 *   contains array('id', 'uid', 'sid', 'date', 'message').
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 *
 * @return array
 *   Return form.
 */
function guestbook_page_form($form, $form_state, $value= NULL) {
  global $user;

  // Get tex for editing and saving data about the message in
  // $form['info_by_id'] for further use.
  $text_edit = '';
  if (isset($value['id'])) {
    $text_edit = $value['message'];
    $form['info_by_id'] = array(
      '#type' => 'value',
      '#value' => $value,
    );
  }

  $path = drupal_get_path('module', 'guestbook');
  $form['#attached']['css'][] = $path . '/css/guestbook.css';

  // Show messages if it exists.
  $variables = guestbook_read_db('guestbook_drupal', 'id', 'DESC', '10');
  $form['message_window'] = array(
    '#theme' => 'guestbook_message_template',
    '#body' => $variables,
  );

  // Show area for input text if user is logged.
  if ($user->uid) {
    $form['message'] = array(
      '#type' => 'textarea',
      '#title' => t('message'),
      '#size' => 1000,
      '#default_value' => $text_edit,
      '#element_validate' => array('guestbook_message_validate'),
    );

    // Show save or edit button.
    $form['submit'] = array(
      '#type' => 'submit',
      '#attributes' => array(
        'class' => array('guestbook-button'),
        ),
      '#value' => isset($value['id']) ? t('Edit') : t('Save'),
    );

    // Show or hide the cancel button.
    if (isset($value['id'])) {
      $form['cancel_button'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => 'guestbook-page',
        '#options' => array(
          'query' => drupal_get_query_parameters(),
          ),
        '#attributes' => array(
          'class' => array('guestbook-button'),
        ),
      );
    }
  }

  return $form;
}

/**
 * Validates guestbook_page_form.
 */
function guestbook_message_validate($element, $form, &$form_state) {
  if (empty($element['#value'] || drupal_strlen($element['#value']) > 1000)) {
    form_error($element, t('Field "@fieldname" must be less than @c 
    chars and can\'t be empty.', array(
      '@fieldname' => t('message'),
      '@c' => 1000,
      ))
    );
  }
}

/**
 * Submit callback for guestbook_page_form.
 */
function guestbook_page_form_submit($form, &$form_state) {
  guestbook_save_edit_db($form_state);
  $form_state['redirect'] = array(
    'guestbook-page', array(
      'query' => drupal_get_query_parameters(),
    )
  );
}

/**
 * Form constructor for the message deletion confirmation form.
 *
 * @param $value
 *   contains array('id', 'uid', 'sid', 'date', 'message').
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 *
 * @return array
 *   Return form.
 */
function guestbook_message_form_delete($form, &$form_state, $value) {
  $form = array();
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $value['id'],
  );
  $query = drupal_get_query_parameters();
  $path = array(
    'path' => 'guestbook-page',
    'query' => $query,
  );
  $question = t('Are you sure you want to delete message?');
  $undone = t('This action cannot be undone.');
  $delete = t('Delete');
  $cancel = t('Cancel');

  return confirm_form($form, $question, $path, $undone, $delete, $cancel);
}

/**
 * Submit callback for guestbook_message_form_delete.
 */
function guestbook_message_form_delete_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  guestbook_message_delete($id);
}
