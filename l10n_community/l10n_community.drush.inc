<?php
/**
 * Created for Drupal Wiki.
 * User: eugenmayer
 * Date: 29.05.12
 * Time: 20:47
 */

function l10n_community_drush_command() {
  $items['l10n_po_import'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'description' => 'Import a po file',
    'arguments' => array(
      'filepath' => 'filepath',
      'langcode' => 'langcode, like de, uk and so forth'
    ),
    'options' => array(
      'uid' => 'uid of the user to be the author of the bulk import. Defaults to 1',
      'is_suggestion' => 'should those translations rather end up being suggestion or approved translations?.Defaults to no',
      'recurse' => 'accept folder, recurse of the po file found there'
    ),
    'alias' => 'l10n_import',
  );
  return $items;
}

function drush_l10n_community_l10n_po_import() {
  $args = drush_get_arguments();
  $fp = $args[1];
  $langcode = $args[2];
  $uid = drush_get_option('uid', 1);
  $is_suggestion = drush_get_option('is_suggestion', 0);
  if(!is_dir($fp)) {
    drush_l10n_community_l10n_po_import_file($fp, $langcode, $uid, $is_suggestion);
  }
  else if(drush_get_option('recurse', FALSE)) {
    $files = file_scan_directory($fp, 'po');
    foreach($files as $file) {
      drush_l10n_community_l10n_po_import_file($file->filename, $langcode, $uid, $is_suggestion);
    }
  }
  else {
    drush_log('you did not set the recurse flag, but a folder was given, instead of a filepath','error');
  }
}

function drush_l10n_community_l10n_po_import_file($filepath, $langcode, $uid = 1, $is_suggestion = 0) {
  if (empty($filepath) || !file_exists($filepath)) {
    drush_log("You need to enter a file to import. Check if the file exists ($filepath)", 'error');
    return;
  }
  if (empty($langcode) || strlen($langcode) > 2) {
    drush_log('The second parameter must be a langcode, like de, uk', 'error');
    return;
  }
  drush_log("importing file: $filepath", 'ok');
  drush_log("destination langcode is: $langcode", 'ok');
  $file = new stdClass();
  $file->filepath = $filepath;
  require_once drupal_get_path('module', 'l10n_community') . '/import.inc';
  if (l10n_server_parse_po($file, '_l10n_community_import_one_string', array($langcode, $is_suggestion, $uid))) {
    drush_log('imported successfully', 'success');
  }
  else {
    drush_log('unknow errors occurred :D', 'error');
  }
}