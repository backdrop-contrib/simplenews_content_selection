<?php
//$Id$
  
/**
 * @file
 * Select Drupal content to create a newsletter
 */

/**
 * Theme the node selection form
 */
function theme_scs_node_selection(&$form) {
  $form = $form['form'];
  
  if ($form['step']['#value'] == 1) {
    //Headers
    $headers = array(
      '',
      array(
        'data' => t('Nid'),
        'field' => 'nid',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Title'),
        'field' => 'title',
      ),
      array(
        'data' => t('Created'),
        'field' => 'created',
      ),
    );
    
    $nodes = _scs_get_nodes($headers);

    $data = array();
      
    $explain = '';
      
    if (user_access('administer scs')) {
      $explain = '<div class="form-item">' . t('Want to select more nodes? Add more content types ') . l(t('here'), 'admin/settings/simplenews/scs') . '.' . '</div>';
    }
  
    foreach ($nodes as $node) {
      if (array_key_exists('nid_' . $node->nid, $form)) {
        $nodearray = array();
        $nodearray[] = drupal_render($form['nid_' . $node->nid]);
        $nodearray[] = $node->nid;
        $nodearray[] = $node->title;
        $nodearray[] = $node->created;
            
        $data[] = $nodearray;
      }
    }

    return drupal_render($form['newsletter_title']) . drupal_render($form['newsletter_toc']) . $explain . drupal_render($form['filters']) . theme('table', array('header' => $headers, 'rows' => $data)) . drupal_render_children($form);
  } 
  else {
    return theme('scs_sort_nodes', $form);
  }
}
  
/**
 * Each selected node goes true this function to create a nice body
 */
function theme_scs_node_output($node) {
  //TODO Fix a teaser of this node
  $output = '';
  $output = '<div id="node_' . $node['nid'] . '">';
    $output .= '<h1>' . $node['title'] . '</h1>';
    $output .= '<p>' . $node['body'][LANGUAGE_NONE][0]['value'] . '</p>';
    $output .= '<p>' . l(t('Read more'), 'node/' . $node['nid']) . '</p>';
  $output .= '</div>';
  
  return $output;
}
  
/**
 * Theme the node sort form into a table
 */
function theme_scs_sort_nodes($form) {
  $form = $form['form'];
  $headers = array(t('Node title'), t('Weight'));
  $rows = array();

  foreach ($form as $name=>$field) {
    if (ereg('weight_', $name)) {
      $nid = explode('_', $name);
      $nid = $nid[1];
      $title = db_query('SELECT title FROM {node} WHERE nid = :nid', array(':nid' => $nid))->fetchField();
      unset($form[$name]['#title']);
      
      $row = array();
      $row[] = $title;
      $row[] = drupal_render($form[$name]);
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
      );
    }
  }
    
  return drupal_render($form['newsletter_title']) . drupal_render($form['newsletter_toc']) . drupal_render($form['newsletter_content_type']) . theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'scs_node_sort_table'))) . drupal_render_children($form);
}
  
/**
 * Theme function to add the titles of the selected nodes at top of the newsletter
 */
function theme_scs_node_titles($titles) {
  if (variable_get('scs_format', 'plain') == 'plain') {
    return implode("\n", $titles);
  }
  else {
    return '<div id="toc">' . theme('item_list', $titles) . '</div>';
  }
}

/**
 * Theme a complete newsletter.
 */
function theme_scs_newsletter_output($variables) {
  $body = '';
  $titles = array();

  // Node information
  foreach ($variables['nodes'] as $node) {
    if ($variables['toc']) {
      if (variable_get('scs_format', 'plain') == 'plain') {
        $titles[] = $node->title;
      }
      else {
        $titles[] = '<a href="#node_' . $node->nid . '">' . $node->title . '</a>';
      }
    }

    $body .= theme('scs_node_output', (array)$node);
  }

  // ToC (if required)
  if ($variables['toc']) {
    $body = theme('scs_node_titles', $titles) . $body;
  }

  // Complete newsletter body
  return $body;
}