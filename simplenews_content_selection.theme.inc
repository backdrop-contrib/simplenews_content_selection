<?php
  // $Id$
  
  /**
   * @file
   * 
   * Handles everything what has to do with theming of this module
   */

  /**
   * Themes a newsletter item
   */
  function theme_simplenews_content_selection_newsletter_item($node) {
    $output = '';
    
    $output .= '<div>';
      $output .= '<h1>'. $node->title .'</h1>';
    $output .= '</div>';
    
    return $output;
  }
  
  /**
   * Themes the selection form
   */
  function theme_simplenews_content_selection_selectlist($form) {
    jquery_plugin_add('tablesorter');
    drupal_add_js(drupal_get_path('module', 'simplenews_content_selection') .'/simplenews_content_selection.js');
    drupal_add_css(drupal_get_path('module', 'simplenews_content_selection') .'/simplenews_content_selection.css');
    
    $output = '';
    
    $output .= drupal_render($form['general']);
    
    $output .= '<fieldset>';
    $output .= '<legend>'. t('Select nodes') .'</legend>';
    $output .= '<table>';
    
    $headers = array('', t('Title'), t('Type'), t('Created'));
    $rows = array();
    
    foreach ($form['nodes'] as $field => $input) {
      if (ereg('nid_', $field)) {
        $form['nodes'][$field]['#title'] = '';
        
        $node = explode('_', $field);
        $node = $node[1];
        $node = node_load(array('nid' => $node));
        $type = content_types($node->type);
        
        $row = array();
        $row[] = drupal_render($form['nodes'][$field]);
        $row[] = $node->title;
        $row[] = $type['name'];
        $row[] = date('Y-m-d H:i:s', $node->created);

        $rows[] = $row;
      }
    }
    
    $output .= theme('table', $headers, $rows, array('id' => 'select_nodes'));
    
    $output .= '</fieldset>';
    
    unset($form['nodes']);

    $output .= drupal_render($form);
    
    return $output;
  }