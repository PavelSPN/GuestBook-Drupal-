<?php
/**
 * @file
 * Implementation of user functions.
 */

/**
 * Get current time.
 *
 * @return
 *   Return time stamp.
 */
function guestbook_time() {
  $date = new DateTime();
  return $date->getTimestamp();
}
