<?php

/**
 * Implementation of hook_views_data().
 */
function uxg_views_data() {
  $data['uxg_field']['table']['group'] = t('UXG field');

  $data['uxg_field']['table']['join'] = array(
    'node' => array(
      'left_field' => 'uuid',
      'field' => 'uuid',
    ),
  );

  $data['uxg_field']['value'] = array(
    'title' => t('Value'),
    'help' => t('UXG field value'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
}
