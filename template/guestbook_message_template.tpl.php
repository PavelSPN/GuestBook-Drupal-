<?php

/**
 * @file
 * Default theme implementation for guestbook messages.
 *
 * Available variables:
 * - $variables['body']: an array containing the following information:
 *   - 'id' - message id;
 *   - 'uid' - user id;
 *   - 'sid' - session id;
 *   - 'date' - unix timestamp;
 *   - 'message' - message text;
 *   - 'user_name' - author of message;
 *   - 'formatted_date' - formatted unix timestamp;
 *   - 'user_object' - an object global $user.
 *
 * @see theme_pager()
 */
?>

<?php if (isset($variables['user_object'])): ?>
  <?php $user = $variables['user_object']; ?>
<?php endif; ?>

<div>
  <?php foreach ($variables['body'] as $value): ?>
    <div class = "guestbook-message">
      <div class = "guestbook-user-date">
        <?php print $value['user_name']; ?>
        <?php print $value['formatted_date']; ?>
      </div>

      <div class = "guestbook-message-text">
        <?php print $value['message']; ?>
      </div>

      <?php // If user is logged and current session id = stored id
            // in database then generate html markup for
            // edit button and delete button.
      ?>
      <?php if ($user->uid && $value['sid'] == $user->sid): ?>
        <?php $params = drupal_get_query_parameters(); ?>
        <?php $links = l(t('edit'), 'guestbook-page/' . $value['id'] . '/edit',
                array(
                  'attributes' => array(
                    'class' => array('guestbook-button'),
                  ),
                  'query' => $params,
                )
              )
              . l(t('delete'), 'guestbook-page/' . $value['id'] . '/delete',
                array(
                  'attributes' => array(
                    'class' => array('guestbook-button'),
                  ),
                  'query' => $params,
                )
              ); ?>

          <div>
            <?php print $links ?>
          </div>

      <?php endif; ?>
    </div>
  <?php endforeach;  ?>
</div>

<div>
  <?php print theme('pager', $variables); ?>
</div>
