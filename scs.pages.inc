<?php

/**
 * @file
 * Select Drupal content to create a newsletter
 */

/**
 * Page callback: Node selection page also known as the first way
 * to use this module
 */
function scs_node_selection($form, &$form_state) {
  // Prevent the user from using this page if there is no content type
  // enabled for selection
  if (!variable_get('scs_content_types', FALSE)) {
    drupal_set_message(t('You need to enable some content types for selection in order to use this module. You can enable them in !link.', array('!link' => l(t('Simplenews Content Selection settings'), 'admin/config/simplenews/scs'))), 'error');
    return "";
  }

  if (!isset($form_state['scs'])) {
    // This is the first step of our two-step form
    $form['step'] = array(
      '#type' => 'hidden',
      '#value' => 1,
    );

    // Newsletter-related elements
    $form['newsletter_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('Enter the title of this newsletter'),
      '#default_value' => variable_get('scs_default_title', ''),
      '#required' => TRUE,
    );

    $form['newsletter_toc'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create a table of contents'),
      '#description' => t('Create a table of contents at top of the newsletter with the titles of the selected nodes. If newsletter is HTML format, these table of contents will be bookmarks.'),
    );

    // Now begins the real content selection

    // Filtering
    $form['filters'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filters'),
      '#collapsible' => TRUE,
      '#collapsed' => (empty($filters) ? TRUE : FALSE),
    );
    $form['filters']['nid'] = array(
      '#type' => 'textfield',
      '#title' => t('Nid'),
      '#description' => t('Filter on a single nid.'),
      '#default_value' => $form_state['values']['nid'] ? : '',
    );
    $form['filters']['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('Filter on title, matching the first characters.'),
      '#default_value' => $form_state['values']['title'] ? : '',
    );
    $types = array(t('None')) + drupal_map_assoc(array_filter(variable_get('scs_content_types', array())));
    $form['filters']['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => $types,
      '#multiple' => TRUE,
      '#default_value' => $form_state['values']['type'] ? : array_keys($types),
    );
    $form['filters']['filter'] = array(
      '#type' => 'submit',
      '#value' => t('Filter'),
    );


    // Table header
    $header = array(
      'title' => array('data' => t('Title'), 'field' => 'title', 'sort' => 'ASC'),
      'type' => array('data' => t('Type'), 'field' => 'type'),
      'created' => array('data' => t('Created'), 'field' => 'created'),
    );

    // Rows
    $rows = array();
    $query = db_select('node', 'n')
      ->extend('TableSort')
      ->fields('n', array('nid', 'title', 'type', 'created'))
      ->orderByHeader($header);
    if (!empty($form_state['input'])) {
      if (!empty($form_state['input']['nid'])) {
        $query->condition('nid', $form_state['values']['nid']);
      }
      if (!empty($form_state['input']['title'])) {
        $query->condition('title', $form_state['input']['nid'] . '%', 'LIKE');
      }
    }
    if (!empty($form_state['input']['type'])) {
      $query->condition('type', $form_state['input']['type']);
    }
    else {
      $query->condition('type', array_keys($types));
    }
    $results = $query->execute();

    foreach ($results as $node) {
      $rows[$node->nid] = array(
        'title' => l($node->title, 'node/' . $node->nid . '/edit'),
        'type' => $node->type,
        'created' => format_date($node->created),
      );
    }
    $form['nodes'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#empty' => t('No nodes found'),
    );

    // Submit button
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Sort nodes'),
    );
  }
  else {
    $form['step'] = array(
      '#type' => 'hidden',
      '#value' => 2,
    );

    $form['nodes']['#tree'] = TRUE;
    // Create elements for the tablesort
    foreach (array_filter($form_state['values']['nodes']) as $nid) {
      $form['nodes'][$nid] = array(
        'weight' => array(
          '#type' => 'weight',
          '#attributes' => array('class' => array('node-weight')),
        ),
      );
    }

    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create newsletter'),
    );
  }

  return $form;
}

function scs_node_selection_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Sort nodes')) {
    $nodes = array_filter($form_state['values']['nodes']);
    if (empty($nodes) && $form_state['values']['step'] == 1) {
      form_set_error('nodes', t('You need to select at least one node.'));
    }
  }
}

function scs_node_selection_submit($form, &$form_state) {
  if ($form_state['values']['step'] == 1) {
    $form_state['rebuild'] = TRUE;
    if ($form_state['values']['op'] == t('Sort nodes')) {
      // Next step
      $form_state['scs']['step'] = 2;
    }
  }
  else {
    $nodes = array_keys($form_state['values']['nodes']);

    //TODO Type has to be fixed here
    $type = 'simplenews';
    _scs_create_newsletter($form_state['scs']['title'], $nodes, $form_state['scs']['toc'], $type);
  }
}

/**
 * Node sort page
 */
function scs_sort_nodes($nodes = array()) {
  drupal_add_tabledrag('scs_node_sort_table', 'order', 'sibling', 'scs_weight');
  $fields = false;
  if (array_key_exists('storage', $nodes)) {
    if (count($_GET) != 0) {
      $fields = true;
      $nodes = array();
      foreach ($_GET as $key => $nid) {
        if ($key != 'q') {
          $nodes[] = $nid;
        }
      }
    }
    else {
      drupal_set_message(t('There was an error. No nodes found'));
      return array();
    }
  }

  $form = array();

  if ($fields == TRUE) {
    $form['newsletter_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#description' => t('Enter the title of this newsletter'),
      '#default_value' => variable_get('scs_default_title', t('Please edit the title of this newsletter.')),
      '#required' => true,
    );

    $form['newsletter_toc'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create a table of contents'),
      '#description' => t('Create a table of contents at top of the newsletter with the titles of the selected nodes. If newsletter is HTML format, these table of contents will be bookmarks.'),
    );

    $content_types = variable_get('simplenews_content_types', array());

    if (count($content_types) != 1) {
      $form['newsletter_content_type'] = array(
        '#type' => 'select',
        '#title' => t('Content type'),
        '#description' => t('Select the content type of the newsletter you are creating. Only the content types selected ' . l(t('here'), 'admin/settings/simplenews/general') . ' are displayed.'),
        '#options' => $content_types,
      );
    }
  }

  $counter = 0;

  foreach ($nodes as $nid) {
    $form['weight_' . $nid] = array(
      '#type' => 'select',
      '#title' => t('Weight'),
      '#options' => array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10),
      '#default_value' => $counter,
      '#attributes' => array('class' => array('scs_weight')),
    );

    $counter++;
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create newsletter'),
    '#submit' => array('scs_sort_nodes_submit'),
  );

  return $form;
}

/**
 * Submit function of the node sorter page
 */
function scs_sort_nodes_submit($form, &$form_state) {
  $nodes = array();

  foreach ($form_state['values'] as $key => $value) {
    if (preg_match('/^weight_/', $key)) {
      $nid = explode('_', $key);
      $nid = $nid[1];
      $nodes[$nid] = $value;
    }
  }

  //Content type
  if (array_key_exists('newsletter_content_type', $form_state['values'])) {
    $type = $form_state['values']['newsletter_content_type'];
  }
  else {
    $types = variable_get('simplenews_content_types', array());
    foreach ($types as $selected) {
      $type = $selected;
    }
  }

  asort($nodes);
  $nodes = array_keys($nodes);

  _scs_create_newsletter($form_state['values']['newsletter_title'], $nodes, $form_state['values']['newsletter_toc'], $type);
}
