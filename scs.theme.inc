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
  	
  	//$headers = array('', t('Nid'), t('Title'), t('Created'));
  	$headers = array(
  		array('data' => ''),
  		array('data' => t('Nid')),
  		array('data' => t('Title')),
  		array('data' => t('Created')),
  	);
  	
  	$table = theme('table', array(
  		'header' => $headers,
  		'rows' => $data,
  	));
  	
  	//return drupal_render($form['newsletter_title']) . $explain . $table . drupal_render($form);
  	return $table;
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
  
  /**
   * Theme the node sort form into a table
   */
  function theme_scs_sort_nodes($form) {
    $headers = array(t('Node title'), t('Weight'));
    $rows = array();

    foreach ($form as $name=>$field) {
      if (ereg('weight_', $name)) {
        $nid = explode('_', $name);
        $nid = $nid[1];
        $title = db_result(db_query('SELECT title FROM {node} WHERE nid = %d', $nid));
        unset($form[$name]['#title']);
        
        $row = array();
        $row[] = $title;
        $row[] = drupal_render($form[$name]);
        $rows[] = $row;
      }
    }
    
    return theme('table', $headers, $rows) . drupal_render($form);
  }
  
  /**
   * Theme function to add the titles of the selected nodes at top of the newsletter
   */
  function theme_scs_node_titles($titles) {
  	if (variable_get('scs_format', 'plain') == 'plain') {
  		return implode("\n", $titles);
  	} else {
  		return '<div id="toc">' . theme('item_list', $titles) . '</div>';
  	}
  }