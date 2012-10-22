<?php

/**
 * @file
 * Integrate Simplenews Content Selection with views and views bulk operations
 */

/**
 * Theme the selection form
 */
function theme_scs_views_selection_form($form) {
  $output = drupal_render($form['newsletter_title']);

  $output .= drupal_render($form);
}
