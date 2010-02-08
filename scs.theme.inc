<?php
  //$Id$
  
  /*
   * @file
   * Select Drupal content to create a newsletter
   */

  /*
   * Theme the node selection form
   */
  function theme_scs_node_selection($form) {
  	$nodes = _scs_get_nodes();
  	
  	$data = array();
  	
  	$explain = '';
  	
  	if (user_access('administer scs')) {
  		$explain = '<div class="form-item">' . t('Want to select more nodes? Add more content types ') . l('here', 'admin/settings/simplenews/scs') . '.' . '</div>';
  	}

  	foreach ($nodes as $node) {
  		$nodearray = array();
  		$nodearray[] = drupal_render($form['nid_' . $node->nid]);
  		$nodearray[] = $node->nid;
  		$nodearray[] = $node->title;
  		$nodearray[] = $node->created;
  		
  		$data[] = $nodearray;
  	}
  	
  	$headers = array('', t('Nid'), t('Title'), t('Created'));
  	
  	return drupal_render($form['newsletter_title']) . $explain . theme('table', $headers, $data) . drupal_render($form);
  }
  
  /*
   * Each selected node goes true this function to create a nice body
   */
  function theme_scs_node_output($node) {
  	$output = '';
  	
  	$output = '<div id="node_' . $node->nid . '">';
  		$output .= '<h1>' . $node->title . '</h1>';
  		$output .= '<p>' . node_teaser($node->body) . '</p>';
  		$output .= '<p>' . l(t('Read more'), 'node/' . $node->nid) . '</p>';
  	$output .= '</div>';
  	
  	return $output;
  }