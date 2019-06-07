<?php
/**
 * @file
 * The implementation of the functions of the weather block.
 */

/**
 * Get xml file by url and store it in locale file.
 * The file is saved in the root folder of the module.
 */
function guestbook_get_xml_weather() {
  // Get xml file by url.
  $xml = file_get_contents("https://xml.meteoservice.ru/export/gismeteo/point/2897.xml");

  // Store xml file in locale file.
  if (isset($xml)){
    $path = drupal_get_path('module', 'guestbook');
    file_put_contents($path . '/weather.xml', $xml);
  }
}

/**
 * Reading data from an xml file and converting it into html.
 *
 * @return
 *   Html markup.
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
      $date = (string) $xml -> REPORT -> TOWN -> FORECAST[0]['day'] . '.' . $xml -> REPORT -> TOWN -> FORECAST[0]['month']  . '.' .
        $xml -> REPORT -> TOWN -> FORECAST[0]['year'];

      $city = 'Витебск';

      // Store information in array.
      $weather = array(
        'city' => $city,
        'date' => $date,
        'temp_max' => $tmax,
        'temp_min' => $tmin,
      );

      // Create an html data of weather.
      $data = '<div>' . $weather['city'] . '</div>';
      $data .= '<p>' . $weather['date'] . '</p>';
      $data .= '<div>Макс. темп. &nbsp  ' . $weather['temp_max'] . '&deg</div>';
      $data .= '<div>Мин. темп. &nbsp ' . $weather['temp_min'] . '&deg</div>';
    }
    else {
      $data = 'Погода временно не доступна';
    }

  }
  else {
    $data = 'Погода временно не доступна';
  }
  return '<div>' . $data . '</div>';
}
