<?php

/**
 * @file
 * Build form for guestbook.
 */

/**
 * Show this notification if the message is incorrect.
 */
define("MESSAGE_ERROR", 'Field "@fieldname" must be less than @c 
chars and can\'t be empty.');

/**
 * Guestbook page form.
 *
 * @param $value
 *   contains array('id', 'uid', 'sid', 'date', 'message').
 * @return
 *   Return form.
 */
function guestbook_page_form($form, $form_state, $value= NULL) {
  global $user;
  // Text for editing.
  $text_edit = '';

  // Get tex for editing and saving data about the message in
  // $form['info_by_id'] for further use.
  if (isset($value['id'])) {
    $text_edit = $value['message'];
    $form['info_by_id'] = array(
      '#type' => 'value',
      '#value' => $value,
      );
  }

  $form['#attached']['css'][] = drupal_get_path('module','guestbook')
    . '/css/guestbook.css';

  // Show messages if it exists.
  $variables = guestbook_read_db('guestbook_drupal', 'id', 'DESC', '10');
  $form['message_window'] = [
    '#theme' => 'guestbook_message_template',
    '#body' => $variables,
  ];

  // Show area for input text if user is logged.
  if ($user->uid) {
    $form['message'] = [
      '#type' => 'textarea',
      '#title' => t('message'),
      '#size' => 1000,
      '#default_value' => $text_edit,
      '#element_validate' => array('guestbook_message_validate'),
    ];

    // Show save or edit button.
    $form['submit'] = [
      '#type' => 'submit',
      '#attributes' => array(
        'class' => array('guestbook-button'),
        ),
      '#value' => isset($value['id']) ? t('Edit') : t('Save'),
    ];

    // Show or hide the cancel button.
    if (isset($value['id'])) {
      $form['cancel_button'] = [
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => 'guestbook-page',
        '#options' => array(
          'query' => drupal_get_query_parameters(),
          ),
        '#attributes' => array(
          'class' => array('guestbook-button'),
          ),
      ];
    }
  }

  return $form;
}

/**
 * Validate guestbook_page_form.
 */
function guestbook_message_validate($element, $form, &$form_state) {
  if (drupal_strlen($element['#value']) > 1000 || empty($element['#value'])) {
    form_error($element, t(MESSAGE_ERROR,
      array(
        '@fieldname' => t('message'),
        '@c'         => 1000))
    );
  }
}

/**
 * Submit for guestbook_page_form.
 */
function guestbook_page_form_submit($form, &$form_state) {
  guestbook_save_edit_db($form_state);
  $form_state['redirect'] = array(
    'guestbook-page', array(
      'query' => drupal_get_query_parameters(),
    )
  );
}
