<?php

/**
 * @file
 * Implementation of core functions.
 */

module_load_include('inc', 'guestbook', 'include/guestbook.db');

/**
 * Gets current time.
 *
 * @return string
 *   Return time stamp.
 */
function guestbook_time() {
  $date = new DateTime();

  return $date->getTimestamp();
}

/**
 * Get xml file by url.
 *
 * The file is saved in the root folder of the module.
 */
function guestbook_get_xml_weather() {
  // Get xml file by url.
  $xml = file_get_contents(WEATHER_URL);

  // Store xml file in locale file for weather block.
  if (isset($xml)) {
    $path = drupal_get_path('module', 'guestbook');
    file_put_contents($path . '/weather.xml', $xml);
  }
}

/**
 * Reading data from an xml file for weather block.
 *
 * @return array
 *   An array containing the following data:
 *   - 'city' - current city;
 *   - 'date' - weather day, month, year;
 *   - 'temp_max' - maximum temperature on the specified date;
 *   - 'temp_min' - minimum temperature on the specified date;
 *   - 'weather_error' - message 'The weather temporarily unavailable'.
 */
function guestbook_weather() {

  $path = drupal_get_path('module', 'guestbook');

  libxml_use_internal_errors(true);
  $xml = new DOMDocument();

  // Generates data of weather if 'weather.xml' file is uploaded and valid.
  if (!file_exists($path . '/weather.xml') || !$xml->load($path . '/weather.xml')) {
    $weather['weather_error'] = t('The weather temporarily unavailable');
    return $weather;
  }
  else {
    // Loads simple xml object.
    $xml = simplexml_load_file($path . '/weather.xml');

    // Gets information from object.
    $tmax = (string) $xml->REPORT->TOWN->FORECAST->TEMPERATURE['max'];
    $tmin = (string) $xml->REPORT->TOWN->FORECAST->TEMPERATURE['min'];
    $date = (string) $xml->REPORT->TOWN->FORECAST[0]['day'];
    $date .= (string) '.' . $xml->REPORT->TOWN->FORECAST[0]['month'];
    $date .= (string) '.' . $xml->REPORT->TOWN->FORECAST[0]['year'];

    $city = t('Vitebsk');

    // Stores information in array.
    $weather = array(
      'city' => $city,
      'date' => $date,
      'temp_max' => $tmax,
      'temp_min' => $tmin,
    );
  }

  return $weather;
}
