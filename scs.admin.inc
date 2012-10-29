<?php

/*
 * @file
 * Select Drupal content to create a newsletter
 */

/*
 * Admin settings form
 */
function scs_admin_settings_form() {
  $form = array();

  $form['scs_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#description' => t('Select the content types that must be available for node selection'),
    '#options' => array_map('scs_node_type_labels', node_type_get_types()),
    '#default_value' => variable_get('scs_content_types', array()),
  );

  $form['scs_format'] = array(
    '#type' => 'radios',
    '#title' => t('Newsletter format'),
    '#description' => t('Select the format of the newsletters you want to send'),
    '#options' => array('plain' => t('Plain'), 'html' => t('HTML')),
    '#default_value' => variable_get('scs_format', 'plain'),
  );

  $form['scs_publish_default'] = array(
    '#type' => 'select',
    '#title' => t('Default publish setting for created newsletters'),
    '#description' => t('Change this select to unpublished if you want to not publish the newsletter node at creation.'),
    '#options' => array(
      0 => t('Unpublished'),
      1 => t('Published'),
    ),
    '#default_value' => variable_get('scs_publish_default', 1),
  );

  $form['scs_default_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title for created newsletters'),
    '#description' => t('If filled, this title will be used to populate "title" field when creating a newsletter.'),
    '#default_value' => variable_get('scs_default_title', ''),
  );

  return system_settings_form($form);
}

function scs_node_type_labels($type) {
  return $type->name;
}
