<?php

/**
 * @file
 * Date administration functionality.
 */

/**
 * Settings for the default formatter.
 */
function date_default_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  $date_formats = date_format_type_options();
  $element['format_type'] = array(
    '#title' => t('Choose how users view dates and times:'),
    '#type' => 'select',
    '#options' => $date_formats + array('custom' => t('Custom')),
    '#default_value' => $settings['format_type'],
    '#description' => t('To add or edit options, visit <a href="@date-time-page">Date and time settings</a>.', array('@date-time-page' => url('admin/config/regional/date-time'))),
    '#weight' => 0,
  );

  $element['custom_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom date format'),
    '#description' => t('If "Custom", see the <a href="@url" target="_blank">PHP manual</a> for date formats. Otherwise, enter the number of different time units to display, which defaults to 2.', array('@url' => 'https://www.php.net/manual/en/function.date.php')),
    '#default_value' => isset($settings['custom_date_format']) ? $settings['custom_date_format'] : '',
    '#dependency' => array('edit-options-settings-format-type' => array('custom')),
  );

  $element['fromto'] = array(
    '#title' => t('Display:'),
    '#type' => 'select',
    '#options' => array(
      'both' => t('Both Start and End dates'),
      'value' => t('Start date only'),
      'value2' => t('End date only'),
    ),
    '#access' => $field['settings']['todate'],
    '#default_value' => $settings['fromto'],
    '#weight' => 1,
  );

  // Make the string translatable by keeping it as a whole rather than
  // translating prefix and suffix separately.
  list($prefix, $suffix) = explode('@count', t('Show @count value(s)'));
  $element['multiple_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Multiple values:'),
    '#size' => 5,
    '#field_prefix' => $prefix,
    '#field_suffix' => $suffix,
    '#default_value' => $settings['multiple_number'],
    '#weight' => 2,
    '#access' => ($field['cardinality'] === FIELD_CARDINALITY_UNLIMITED) || ($field['cardinality'] > 1),
    '#description' => t('Identify a specific number of values to display, or leave blank to show all values.'),
  );

  list($prefix, $suffix) = explode('@isodate', t('starting from @isodate'));
  $element['multiple_from'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#field_prefix' => $prefix,
    '#field_suffix' => $suffix,
    '#default_value' => $settings['multiple_from'],
    '#weight' => 3,
    '#access' => ($field['cardinality'] === FIELD_CARDINALITY_UNLIMITED) || ($field['cardinality'] > 1),
  );

  list($prefix, $suffix) = explode('@isodate', t('ending with @isodate'));
  $element['multiple_to'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#field_prefix' => $prefix,
    '#field_suffix' => $suffix,
    '#default_value' => $settings['multiple_to'],
    '#weight' => 4,
    '#access' => ($field['cardinality'] === FIELD_CARDINALITY_UNLIMITED) || ($field['cardinality'] > 1),
    '#description' => t('Identify specific start and/or end dates in the format YYYY-MM-DDTHH:MM:SS, or leave blank for all available dates.'),
  );

  $element['show_remaining_days'] = array(
    '#title' => t('Show remaining days'),
    '#type' => 'checkbox',
    '#default_value' => $settings['show_remaining_days'],
    '#weight' => 0,
  );

  return $element;
}

/**
 * Settings for the interval formatter.
 */
function date_interval_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();
  $element['interval'] = array(
    '#title' => t('Interval'),
    '#description' => t("How many time units should be shown in the 'time ago' string."),
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(1, 6)),
    '#default_value' => $settings['interval'],
    '#weight' => 0,
  );
  // Uses the same options used by Views format_interval.
  $options = array(
    'raw time ago' => t('Time ago'),
    'time ago' => t('Time ago (with "ago" appended)'),
    'raw time hence' => t('Time hence'),
    'time hence' => t('Time hence (with "hence" appended)'),
    'raw time span' => t('Time span (future dates have "-" prepended)'),
    'inverse time span' => t('Time span (past dates have "-" prepended)'),
    'time span' => t('Time span (with "ago/hence" appended)'),
  );
  $element['interval_display'] = array(
    '#title' => t('Display'),
    '#description' => t("How to display the time ago or time hence for this field."),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $settings['interval_display'],
    '#weight' => 0,
  );
  if (!empty($field['settings']['todate'])) {
    $element['use_end_date'] = array(
      '#title' => t('Use End date'),
      '#description' => 'Use the End date, instead of the start date',
      '#type' => 'checkbox',
      '#default_value' => $settings['use_end_date'],
    );
  }

  return $element;
}

/**
 * Settings summary for the default formatter.
 */
function date_default_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $formatter = $display['type'];
  $format_types = date_format_type_options();
  $summary = array();
  $format = FALSE;
  switch ($formatter) {
    case 'date_plain':
      $format = t('Plain');
      break;

    case 'format_interval':
      $format = t('Interval');
      break;

    default:
      if (!empty($format_types[$settings['format_type']])) {
        $format = $format_types[$settings['format_type']];
      }
  }
  if ($format) {
    $summary[] = t('Display dates using the @format format', array('@format' => $format));
  }
  else {
    $summary[] = t('Display dates using the default format because the specified format (@format) is not defined', array('@format' => $settings['format_type']));
  }

  if (array_key_exists('fromto', $settings) && $field['settings']['todate']) {
    $options = array(
      'both' => t('Display both Start and End dates'),
      'value' => t('Display Start date only'),
      'value2' => t('Display End date only'),
    );
    if (isset($options[$settings['fromto']])) {
      $summary[] = $options[$settings['fromto']];
    }
  }

  if (array_key_exists('multiple_number', $settings) && !empty($field['cardinality'])) {
    $summary[] = t('Show @count value(s) starting with @date1, ending with @date2', array(
      '@count' => !empty($settings['multiple_number']) ? $settings['multiple_number'] : t('all'),
      '@date1' => !empty($settings['multiple_from']) ? $settings['multiple_from'] : t('earliest'),
      '@date2' => !empty($settings['multiple_to']) ? $settings['multiple_to'] : t('latest'),
    ));
  }

  if (array_key_exists('show_remaining_days', $settings)) {
    $summary[] = t('Show remaining days: @value', array('@value' => ($settings['show_remaining_days'] ? 'yes' : 'no')));
  }

  return $summary;
}

/**
 * Settings summary for the interval formatter.
 *
 * @todo Add settings later.
 */
function date_interval_formatter_settings_summary($field, $instance, $view_mode) {
  $summary = array();
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $field_settings = ($settings['use_end_date'] === 1) ? 'End' : 'Start';
  $summary[] = t('Display time ago, showing @interval units. Using @field Date',
    array('@interval' => $settings['interval'], '@field' => $field_settings));

  return $summary;
}

/**
 * Helper function for date_field_instance_settings_form().
 *
 * @see date_field_instance_settings_form_validate()
 */
function _date_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form['default_value'] = array(
    '#type' => 'select',
    '#title' => t('Default date'),
    '#default_value' => $settings['default_value'],
    '#options' => array(
      'blank'     => t('No default value'),
      'now'       => t('Now'),
      'strtotime' => t('Relative'),
    ),
    '#weight' => 1,
    '#fieldset' => 'default_values',
  );

  $description = t("Describe a time by reference to the current day, like '+90 days' (90 days from the day the field is created) or '+1 Saturday' (the next Saturday). See !strtotime for more details.", array('!strtotime' => l(t('strtotime'), 'https://www.php.net/manual/en/function.strtotime.php')));
  $form['default_value_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Relative default value'),
    '#description' => $description,
    '#default_value' => $settings['default_value_code'],
    '#states' => array(
      'visible' => array(
        ':input[name="instance[settings][default_value]"]' => array(
          'value' => 'strtotime',
        ),
      ),
    ),
    '#weight' => 1.1,
    '#fieldset' => 'default_values',
  );
  $form['default_value2'] = array(
    '#type' => !empty($field['settings']['todate']) ? 'select' : 'hidden',
    '#title' => t('Default end date'),
    '#default_value' => $settings['default_value2'],
    '#options' => array(
      'same'      => t('Same as Default date'),
      'blank'     => t('No default value'),
      'now'       => t('Now'),
      'strtotime' => t('Relative'),
    ),
    '#weight' => 2,
    '#fieldset' => 'default_values',
  );
  $form['default_value_code2'] = array(
    '#type' => !empty($field['settings']['todate']) ? 'textfield' : 'hidden',
    '#title' => t('Relative default value for end date'),
    '#description' => $description,
    '#default_value' => $settings['default_value_code2'],
    '#states' => array(
      'visible' => array(
        ':input[name="instance[settings][default_value2]"]' => array(
          'value' => 'strtotime',
        ),
      ),
    ),
    '#weight' => 2.1,
    '#fieldset' => 'default_values',
  );

  $form['#element_validate'] = array('date_field_instance_settings_form_validate');

  $context = array(
    'field' => $field,
    'instance' => $instance,
  );

  // Trigger hook_date_field_instance_settings_form_alter().
  drupal_alter('date_field_instance_settings_form', $form, $context);

  return $form;
}

/**
 * Form validation handler for _date_field_instance_settings_form().
 */
function _date_field_instance_settings_form_validate(&$form, &$form_state) {
  $settings = $form_state['values']['instance']['settings'];

  if ($settings['default_value'] === 'strtotime') {
    $is_strtotime = @strtotime($settings['default_value_code']);
    if (!$is_strtotime) {
      form_set_error('instance][settings][default_value_code', t('The Strtotime default value is invalid.'));
    }
  }

  if (isset($settings['default_value2']) && $settings['default_value2'] === 'strtotime') {
    $is_strtotime = @strtotime($settings['default_value_code2']);
    if (!$is_strtotime) {
      form_set_error('instance][settings][default_value_code2', t('The Strtotime default value for the End Date is invalid.'));
    }
  }
}

/**
 * Helper function for date_field_widget_settings_form().
 *
 * @see date_field_widget_settings_form_validate()
 */
function _date_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form = array(
    '#element_validate' => array('date_field_widget_settings_form_validate'),
  );

  $options = array();
  if ($widget['type'] === 'date_popup' && module_exists('date_popup')) {
    $formats = date_popup_formats();
  }
  else {
    // Example input formats must show all possible date parts, so add seconds.
    $formats = str_replace('i', 'i:s', array_keys(system_get_date_formats('short')));
    $formats = drupal_map_assoc($formats);
  }
  $now = date_example_date();
  $options['site-wide'] = t('Short date format: @date', array('@date' => date_format_date($now, 'short')));
  foreach ($formats as $f) {
    $options[$f] = date_format_date($now, 'custom', $f);
  }
  $form['input_format'] = array(
    '#type' => 'select',
    '#title' => t('Date entry options'),
    '#default_value' => $settings['input_format'],
    '#options' => $options,
    '#description' => t('Control the order and format of the options users see.'),
    '#weight' => 3,
    '#fieldset' => 'date_format',
  );
  // Only a limited set of formats is available for the Date Popup module.
  if ($widget['type'] !== 'date_popup') {
    $form['input_format']['#options']['custom'] = t('Custom format');
    $form['input_format_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom input format'),
      '#default_value' => $settings['input_format_custom'],
      '#description' => t("Override the input format selected above. Define a php date format string like 'm-d-Y H:i' (see <a href=\"@link\">the PHP manual</a> for more details).", array('@link' => 'https://www.php.net/manual/en/function.date.php')),
      '#weight' => 5,
      '#fieldset' => 'date_format',
      '#attributes' => array('class' => array('indent')),
      '#states' => array(
        'visible' => array(
          ':input[name="instance[widget][settings][input_format]"]' => array('value' => 'custom'),
        ),
      ),
    );
  }
  else {
    $form['input_format_custom'] = array(
      '#type' => 'hidden',
      '#value' => '',
    );
  }

  if (in_array($widget['type'], array('date_select', 'date_popup'))) {
    $form['year_range'] = array(
      '#type' => 'date_year_range',
      '#default_value' => $settings['year_range'],
      '#fieldset' => 'date_format',
      '#weight' => 6,
    );
    $form['increment'] = array(
      '#type' => 'select',
      '#title' => t('Time increments'),
      '#default_value' => $settings['increment'],
      '#options' => array(
        1 => t('1 minute'),
        5 => t('5 minute'),
        10 => t('10 minute'),
        15 => t('15 minute'),
        30 => t('30 minute'),
      ),
      '#weight' => 7,
      '#fieldset' => 'date_format',
    );
  }
  else {
    $form['year_range'] = array(
      '#type' => 'hidden',
      '#value' => $settings['year_range'],
    );
    $form['increment'] = array(
      '#type' => 'hidden',
      '#value' => $settings['increment'],
    );
  }

  $form['label_position'] = array(
    '#type' => 'value',
    '#value' => $settings['label_position'],
  );
  $form['text_parts'] = array(
    '#type' => 'value',
    '#value' => $settings['text_parts'],
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#fieldset' => 'date_format',
    '#weight' => 9,
  );

  if ($widget['type'] === 'date_select') {
    $options = array(
      'above' => t('Above'),
      'within' => t('Within'),
      'none' => t('None'),
    );
    $description = t("The location of date part labels, like 'Year', 'Month', or 'Day' . 'Above' displays the label as titles above each date part. 'Within' inserts the label as the first option in the select list and in blank textfields. 'None' doesn't visually label any of the date parts. Theme functions like 'date_part_label_year' and 'date_part_label_month' control label text.");
  }
  else {
    $options = array(
      'above' => t('Above'),
      'none' => t('None'),
    );
    $description = t("The location of date part labels, like 'Year', 'Month', or 'Day' . 'Above' displays the label as titles above each date part. 'None' doesn't visually label any of the date parts. Theme functions like 'date_part_label_year' and 'date_part_label_month' control label text.");
  }

  $form['advanced']['label_position'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => $settings['label_position'],
    '#title' => t('Position of date part labels'),
    '#description' => $description,
  );
  $form['advanced']['text_parts'] = array(
    '#theme' => $widget['type'] === 'date_select' ? 'date_text_parts' : NULL,
  );
  $text_parts = (array) $settings['text_parts'];
  foreach (date_granularity_names() as $key => $value) {
    if ($widget['type'] === 'date_select') {
      $form['advanced']['text_parts'][$key] = array(
        '#type' => 'radios',
        '#default_value' => in_array($key, $text_parts, TRUE) ? 1 : 0,
        '#options' => array(0 => '', 1 => ''),
      );
    }
    else {
      $form['advanced']['text_parts'][$key] = array(
        '#type' => 'value',
        '#value' => (int) in_array($key, (array) $settings['text_parts'], TRUE),
      );
    }
  }

  $form['advanced']['no_fieldset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Render as a regular field'),
    '#default_value' => !empty($settings['no_fieldset']),
    '#description' => t('Whether to render this field as a regular field instead of a fieldset. The date field elements are wrapped in a fieldset by default, and may not display well without it.'),
  );

  $context = array(
    'field' => $field,
    'instance' => $instance,
  );
  // Trigger hook_date_field_widget_settings_form_alter().
  drupal_alter('date_field_widget_settings_form', $form, $context);

  return $form;
}

/**
 * Form validation handler for _date_field_widget_settings_form().
 */
function _date_field_widget_settings_form_validate(&$form, &$form_state) {
  // The widget settings are in the wrong place in the form because of #tree on
  // the top level.
  $settings = $form_state['values']['instance']['widget']['settings'];
  $settings = array_merge($settings, $settings['advanced']);
  unset($settings['advanced']);
  form_set_value(array('#parents' => array('instance', 'widget', 'settings')), $settings, $form_state);

  $widget = &$form_state['values']['instance']['widget'];
  // Munge the table display for text parts back into an array of text parts.
  if (is_array($widget['settings']['text_parts'])) {
    form_set_value($form['text_parts'], array_keys(array_filter($widget['settings']['text_parts'])), $form_state);
  }

  if ($widget['settings']['input_format'] === 'custom' && empty($widget['settings']['input_format_custom'])) {
    form_set_error('instance][widget][settings][input_format_custom', t('Please enter a custom date format, or choose one of the preset formats.'));
  }
}

/**
 * Helper function for date_field_settings_form().
 *
 * @see date_field_settings_validate()
 */
function _date_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];
  $granularity = array();

  $form = array(
    '#element_validate' => array('date_field_settings_validate'),
  );

  // Make sure granularity is in the right format and has no empty values.
  if (!empty($settings['granularity']) && is_array($settings['granularity'])) {
    $granularity = array_filter($settings['granularity']);
  }
  $tz_handling = $settings['tz_handling'];

  $description = t('Select the date attributes to collect and store.');
  if ($has_data) {
    $description .= ' ' . t('Changes to date attributes only effects new or updated content.');
  }
  $options = date_granularity_names();
  $checkbox_year = array(
    '#type' => 'checkbox',
    '#title' => check_plain($options['year']),
    '#value' => 'year',
    '#return_value' => 'year',
    '#disabled' => TRUE,
  );
  unset($options['year']);
  $form['granularity'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Date attributes to collect'),
    '#default_value' => $granularity,
    '#options' => $options,
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
    '#description' => $description,
    'year' => $checkbox_year,
  );

  $description = t('End dates are used to collect duration. E.g., allow an event to start on September 15, and end on September 16.');
  $form['enddate_get'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collect an end date'),
    '#description' => $description,
    '#default_value' => !empty($settings['todate']),
    '#disabled' => $has_data,
  );
  $form['enddate_required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required'),
    '#default_value' => isset($settings['todate']) && $settings['todate'] === 'required',
    '#states' => array(
      'invisible' => array(
        'input[name="field[settings][enddate_get]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $description = t('Select the timezone handling method for this date field.');
  $form['tz_handling'] = array(
    '#type' => 'select',
    '#title' => t('Time zone handling'),
    '#default_value' => $tz_handling,
    '#options' => date_timezone_handling_options(),
    '#description' => $description,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'date') . '/date_admin.js'),
    ),
  );
  // Force this value to hidden because we don't want to allow it to be changed
  // right now, but allow it to be a variable if needed.
  $form['timezone_db'] = array(
    '#type' => 'hidden',
    '#value' => date_get_timezone_db($tz_handling),
  );

  $context = array(
    'field' => $field,
    'instance' => $instance,
    'has_data' => $has_data,
  );

  // Trigger hook_date_field_settings_form_alter().
  drupal_alter('date_field_settings_form', $form, $context);

  return $form;
}

/**
 * Form validation handler for _date_field_settings_form().
 */
function _date_field_settings_validate(&$form, &$form_state) {
  $field = &$form_state['values']['field'];

  if ($field['settings']['tz_handling'] === 'none') {
    form_set_value($form['timezone_db'], '', $form_state);
  }
  else {
    form_set_value($form['timezone_db'], date_get_timezone_db($field['settings']['tz_handling']), $form_state);
  }

  if ($field['settings']['tz_handling'] !== 'none' && !in_array('hour', array_filter($field['settings']['granularity']), TRUE)) {
    form_set_error('field[settings][tz_handling]', t('Dates without hours granularity must not use any timezone handling.'));
  }

  // Extract the correct 'todate' value out of the two end date checkboxes.
  if ($field['settings']['enddate_get']) {
    if ($field['settings']['enddate_required']) {
      $field['settings']['todate'] = 'required';
    }
    else {
      $field['settings']['todate'] = 'optional';
    }
  }
  else {
    $field['settings']['todate'] = '';
  }

  // Don't save the pseudo values created in the UI.
  unset($field['settings']['enddate_get'], $field['settings']['enddate_required']);
}

/**
 * Timezone handling options.
 *
 * The 'none' option will do no timezone conversions and will store and display
 * dates exactly as entered useful in locales or situations where timezone
 * conversions are not working reliably, for dates with no times, for historical
 * dates where timezones are irrelevant, or anytime conversion is unnecessary or
 * undesirable.
 */
function date_timezone_handling_options() {
  return array(
    'site' => t("Site's time zone"),
    'date' => t("Date's time zone"),
    'user' => t("User's time zone"),
    'utc'  => 'UTC',
    'none' => t('No time zone conversion'),
  );
}
