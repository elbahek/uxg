<?php
/**
 * @file
 * Features API integration hooks
 */

/**
 * Implementation of hook_features_export_options().
 */
function uxg_features_export_options() {
  $options = db_select('uxg_field', 'uf')
    ->fields('uf', array('uuid', 'value'))
    ->execute()
    ->fetchAllKeyed();

  return $options;
}

/**
 * Implementation of hook_features_export().
 */
function uxg_features_export($data, &$export, $module_name = '') {
  foreach ($data as $uuid) {
    $export['features']['uxg'][] = $uuid;
  }
}

/**
 * Implementation of hook_features_export_render().
 */
function uxg_features_export_render($module_name, $data, $export = NULL) {
  $uxg_data = db_select('uxg_field', 'uf')
    ->fields('uf', array('uuid', 'value'))
    ->condition('uuid', array_values($data))
    ->execute()
    ->fetchAllKeyed();
  $code = array();
  $code[] = ' $uxg = array();';
  foreach ($uxg_data as $uuid => $value) {
    $code[] = ' $uxg["' . $uuid . '"] = "' . $value . '";';
  }
  $code[] = '';
  $code[] = ' return $uxg;';
  $code = join("\n", $code);

  return array('uxg_features_uxg_values' => $code);
}

/**
 * Implementation of hook_features_rebuild().
 */
function uxg_features_rebuild($module) {
  $items = module_invoke($module, 'uxg_features_uxg_values');
  $present_uuids = db_select('uxg_field', 'uf')
    ->fields('uf', array('uuid'))
    ->execute()
    ->fetchCol();
  if (count(array_diff(array_keys($items), $present_uuids)) > 0) {
    $insert_query = db_insert('uxg_field');
    foreach ($items as $uuid => $value) {
      if (in_array($uuid, $present_uuids)) {
        continue;
      }
      $insert_query->fields(array(
        'uuid' => $uuid,
        'value' => $value,
      ));
    }
    $insert_query->execute();
  }
}

/**
 * Implementation of hook_features_revert().
 */
function uxg_features_revert($module) {
  uxg_features_rebuild($module);
}
