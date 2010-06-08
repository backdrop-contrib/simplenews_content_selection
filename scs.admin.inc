<?php
  //$Id$
  
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
  	  '#description' => t('Select the content types that must be avaiable for node selection'),
  	  '#options' => _scs_get_node_types(),
  	  '#default_value' => variable_get('scs_content_types', array()),
  	);
  	
  	$form['scs_format'] = array(
  	  '#type' => 'radios',
  	  '#title' => t('Newsletter format'),
  	  '#description' => t('Select the format of the newsletters you want to send'),
  	  '#options' => array('plain' => t('Plain'), 'html' => t('HTML')),
  	  '#default_value' => variable_get('scs_format', 'plain'),
  	);
  	
  	$form['scs_titles'] = array(
	  '#type' => 'checkbox',
  	  '#title' => t('Embed title in newsletter and create bookmarks (if HTML)'),
  	  '#description' => t('All titles of the selected nodes will be added at the beginning of the newsletter. If HTML is available, titles will be bookmarks.'),
  	  '#default_value' => variable_get('scs_titles', 0),
  	);
  	
  	return system_settings_form($form);
  }
  
  /*
   * Return avaiable node types in option list
   */
  function _scs_get_node_types() {
  	$types = _node_types_build();
  	$options = array();
  	
  	foreach ($types->types as $name=>$type) {
  		$options[$name] = $type->name;
  	}
  	
  	return $options;
  }