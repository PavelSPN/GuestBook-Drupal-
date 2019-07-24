<?php

/**
 * @file
 * Default theme implementation for guestbook weather.
 *
 * Available variables:
 * - $weather: an array containing the following information:
 *   - 'city';
 *   - 'date';
 *   - 'temp_max';
 *   - 'temp_min';
 *   - 'weather_error' - show message error if weather not available.
 */
?>

<div class = 'guestbook-weather-block'>
  <?php if (isset($weather['weather_error'])): ?>

    <div class = 'guestbook-weather-error'>
      <?php print $weather['weather_error'] ?>
    </div>
    <?php else: ?>

      <div class = 'guestbook-weather-city'>
        <?php print $weather['city'] ?>
      </div>

      <div class = 'guestbook-weather-date'>
        <p>
          <?php print $weather['date'] ?>
        </p>
      </div>

      <div class = 'guestbook-weather-temperature'>
        <div>
          Max. temp. &nbsp <?php print $weather['temp_max'] ?> &deg
        </div>
        <div>
          Мin. temp. &nbsp <?php print $weather['temp_min'] ?> &deg
        </div>
      </div>
    <?php endif; ?>
</div>
