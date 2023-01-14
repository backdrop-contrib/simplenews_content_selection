<?php

/**
 * @file
 * Administration callbacks
 */

/**
 * Form callback: admin settings form
 */
function scs_admin_settings_form() {
  $config = config('scs.settings');
  $form = array();
  $form['#config'] = 'scs.settings';

  // View mode
  $entity_info = entity_get_info();
  $view_modes = array('default' => t('Default'));
  foreach ($entity_info['node']['view modes'] as $machine_name => $view_mode) {
    $view_modes[$machine_name] = $view_mode['label'];
  }

  $form['scs_view_mode'] = array(
    '#type' => 'checkboxes',
    '#title' => t('View mode'),
    '#description' => t('Select the view mode to use when rendering the newsletter, default is the one provided by SCS, that you can customize in the Field UI.'),
    '#options' => $view_modes,
    '#default_value' => $config->get('scs_view_mode'),
  );

  // Node type
  $types = backdrop_map_assoc(simplenews_get_content_types());
  if (count($types) > 1) {
    $type_names = node_type_get_names();
    $form['scs_node_type'] = array(
      '#type' => 'select',
      '#title' => t('Newsletter type'),
      '#description' => t('You have defined multiple content types for newsletters, check the one you want to use with SCS.'),
      '#options' => array_intersect_key($type_names, $types),
      '#default_value' => $config->get('scs_node_type'),
    );
  }
  else {
    // Add the newsletter by default
    $form['scs_node_type'] = array(
      '#type' => 'hidden',
      '#value' => end($types),
    );
  }

  return system_settings_form($form);
}
