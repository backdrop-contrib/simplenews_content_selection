<?php

/**
 * @file
 * Theming preprocesses
 */

/**
 * Theme the node selection form.
 */
function theme_simplenews_content_selection_sortable_table($form) {
  $form = $form['form'];
  $headers = array(t('Node title'), t('Weight'), t('View mode'));
  $rows = array();

  $nids = element_children($form['nodes']);

  // Fetch the titles for each nid
  $titles = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('nid', array_values($nids))
    ->execute()
    ->fetchAllKeyed();

  foreach ($nids as $nid) {
    $row = array();
    $row[] = $titles[$nid];
    $row[] = backdrop_render($form['nodes'][$nid]['weight']);
    $row[] = backdrop_render($form['nodes'][$nid]['view_mode']);
    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }
  $output = theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'simplenews_content_selection-sort-nodes')));
  $info = '';
  if (isset($form['simplenews_content_selection_title']) && isset($form['simplenews_content_selection_toc'])) {
    $info = backdrop_render($form['simplenews_content_selection_title']) . backdrop_render($form['simplenews_content_selection_toc']);
  }
  $output = $info . $output . backdrop_render_children($form);
  backdrop_add_tabledrag('simplenews_content_selection-sort-nodes', 'order', 'sibling', 'node-weight');
  return $output;
}

/**
 * Preprocess for simplenews_content_selection_newsletter.
 */
function template_preprocess_simplenews_content_selection_newsletter(&$vars) {
  // Add Table of Contents (if required)
  if ($vars['toc']) {
    $vars['toc'] = theme('simplenews_content_selection_toc', array('nodes' => $vars['nodes']));
  }

  // Render each node using the correct view mode
  foreach ($vars['nodes'] as $nid => $node) {
    $build = node_view($node, $node->simplenews_content_selection_view_mode);

    // Strip out contextual links
    unset($build['#contextual_links']['node']);

    $vars['nodes'][$nid] = $build;
  }
}

/**
 * Theme the Table of Contents.
 */
function theme_simplenews_content_selection_toc($vars) {
  $titles = array();
  foreach ($vars['nodes'] as $node) {
    $titles[] = $node->title;
  }
  return theme('item_list', array(
    'items' => $titles,
    'title' => t('Table of Contents'),
  ));
}
