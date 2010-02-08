<?php
  //$Id$
  
  /*
   * @file
   * Select Drupal content to create a newsletter
   */

  /*
   * Node selection page
   * First way to use this module
   */
  function scs_node_selection() {
  	$nodes = _scs_get_nodes();
  	
  	$form = array();
  	
  	foreach ($nodes as $node) {
  	  $form['nid_' . $node->nid] = array(
  	    '#type' => 'checkbox',
  	  );
  	}
  	
  	$form['newsletter_title'] = array(
  	  '#type' => 'textfield',
  	  '#title' => t('Title'),
  	  '#description' => t('Enter the title of this newsletter'),
  	  '#required' => true,
  	);
  	
  	$form['submit'] = array(
  	  '#type' => 'submit',
  	  '#value' => t('Create newsletter'),
  	);
  	
  	return $form;
  }
  
  /*
   * Submit function of the node selection page
   */
  function scs_node_selection_submit($form, &$form_state) {
  	//Get the selected nodes
  	$nodes = array();
  	
  	$title = $form_state['values']['newsletter_title'];
  	
  	foreach ($form_state['values'] as $field=>$value) {
  		if (ereg('nid_', $field) && $value == 1) {
  		  $nid = explode('_', $field);
  		  $nodes[] = $nid[1];
  		}
  	}
  	
  	_scs_create_newsletter($title, $nodes);
  }