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
  	
  	unset($_SESSION['scs_title']);
  	unset($_SESSION['scs_nodes']);
  	
  	$_SESSION['scs_title'] = $title;
  	$_SESSION['scs_nodes'] = $nodes;
  	
  	drupal_goto('admin/content/scs_sort_nodes');
  }
  
  /**
   * Node sort page
   */
  function scs_sort_nodes() {
    if (isset($_SESSION['scs_nodes'])) {
      $nodes = $_SESSION['scs_nodes'];
      $form = array();
      
      if (isset($_SESSION['scs_title'])) {
        $form['scs_title'] = array(
          '#type' => 'hidden',
          '#value' => $_SESSION['scs_title'],
        );
      }
      
      foreach ($nodes as $nid) {
        $form['weight_' . $nid] = array(
          '#type' => 'select',
          '#title' => t('Weight'),
          '#options' => array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10),
          '#default_value' => 0, 
        );
      }
      
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Create newsletter'),
      );
      
      return $form;
    } else {
      drupal_set_message(t('There was an error. No nodes found'));
      return array();
    }
  }
  
  /**
   * Submit function of the node sorter page
   */
  function scs_sort_nodes_submit($form, &$form_state) {
    $nodes = array();
    
    foreach($form_state['values'] as $key=>$value) {
      if (ereg('weight_', $key)) {
        $nid = explode('_', $key);
        $nid = $nid[1];
        $nodes[$nid] = $value;
      }
    }
    
    asort($nodes);
    $nodes = array_keys($nodes);
    
    $title = '';
    
    if (isset($form_state['values']['scs_title'])) {
      $title = $form_state['values']['scs_title'];
      unset($_SESSION['scs_title']);
    }
    _scs_create_newsletter($title, $nodes);
  }