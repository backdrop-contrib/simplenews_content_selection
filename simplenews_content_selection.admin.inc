<?php
  // $Id$
  
  /**
   * @file
   * 
   * Handles everything what has to do with the admin section of this module
   */

  /**
   * Admin settings form
   */
  function simplenews_content_selection_admin_settings() {
    $form = array();
    
    $form['scs_content_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Content types'),
      '#description' => t('Select the content types you want to have available for content selection'),
      '#options' => _simplenews_content_selection_cck_options(),
      '#default_value' => variable_get('scs_content_types', array()),
    );
    
    return system_settings_form($form);
  }
  
  /**
   * Interal to filter an option array with available content types
   */
  function _simplenews_content_selection_cck_options() {
    $ct = content_types();
    $types = array();
    
    foreach ($ct as $name => $type) {
      $types[$name] = $type['name'];
    }
    
    return $types;
  }