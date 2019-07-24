<?php

/**
 * @file
 * Implementation of user functions.
 */

module_load_include('inc', 'guestbook', 'include/guestbook_db');

/**
 * URL for downloading xml weather file.
 */
define("WEATHER_URL", 'https://xml.meteoservice.ru/export/gismeteo/point/2897.xml');

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

/**
 * Get xml file by url and store it in locale file.
 * The file is saved in the root folder of the module.
 */
function guestbook_get_xml_weather() {
  // Get xml file by url.
  $xml = file_get_contents(WEATHER_URL);

  // Store xml file in locale file.
  if (isset($xml)) {
    $path = drupal_get_path('module', 'guestbook');
    file_put_contents($path . '/weather.xml', $xml);
  }
}

/**
 * Reading data from an xml file.
 *
 * @return
 *   An array containing the following data:
 *   - 'city' - current city;
 *   - 'date' - weather day, month, year;
 *   - 'temp_max' - maximum temperature on the specified date;
 *   - 'temp_min' - minimum temperature on the specified date;
 *   - 'weather_error' - message 'The weather temporarily unavailable'.
 */
function guestbook_weather() {

  $path = drupal_get_path('module', 'guestbook');

  // Generate html data of weather if 'weather.xml' file is uploaded and valid.
  if (file_exists($path . '/weather.xml')) {

    libxml_use_internal_errors(true);
    $xml = new DOMDocument();

    if ($xml->load($path . '/weather.xml')) {
      // Load simple xml object.
      $xml = simplexml_load_file($path . '/weather.xml');

      // Get information from object.
      $tmax = (string) $xml -> REPORT -> TOWN -> FORECAST -> TEMPERATURE['max'];
      $tmin = (string) $xml -> REPORT -> TOWN -> FORECAST -> TEMPERATURE['min'];
      $date = (string) $xml -> REPORT -> TOWN -> FORECAST[0]['day'] . '.' .
        $xml -> REPORT -> TOWN -> FORECAST[0]['month']  . '.' .
        $xml -> REPORT -> TOWN -> FORECAST[0]['year'];

      $city = t('Vitebsk');

      // Store information in array.
      $weather = array(
        'city' => $city,
        'date' => $date,
        'temp_max' => $tmax,
        'temp_min' => $tmin,
      );
    }
    else {
      $weather['weather_error'] = t('The weather temporarily unavailable');
    }
  }
  else {
    $weather['weather_error'] = t('The weather temporarily unavailable');
  }

  return $weather;
}
