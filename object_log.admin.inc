<?php
/**
 * Menu callback function for admin/reports/object_log.
 */
function object_log_overview_page() {
  $build = array();
  $build['object_log_clear'] = drupal_get_form('object_log_clear_form');
  $build['object_log'] = object_log_build_log();
  return $build;
}

/**
 * Build a render array showing objects stored in the object log.
 *
 * @param $cid
 *   If set, create links for comparing the selected object with the given CID.
 * @return
 *   A render array for a table of links to stored objects.
 */
function object_log_build_log($cid = NULL) {
  $rows = array();
  $header = array(
    array(
      'data' => t('CID'),
      'field' => 'c.cid',
    ),
    array(
      'data' => t('Date'),
      'field' => 'c.created',
      'sort' => 'desc',
    ),
  );
  $query = db_select(OBJECT_LOG_TABLE, 'c')->extend('PagerDefault')->extend('TableSort');
  $query->fields('c', array('cid', 'created'));
  $result = $query
    ->limit(25)
    ->orderByHeader($header)
    ->execute();
  
  foreach ($result as $item) {
    if ($cid) {
      $link = l($item->cid, OBJECT_LOG_ADMIN_PATH . '/' . $cid . '/' . $item->cid);
    }
    else {
      $link = l($item->cid, OBJECT_LOG_ADMIN_PATH . '/' . $item->cid);
    }
    $rows[] = array(
      $link,
      format_date($item->created, 'short'),
    );
  }
  
  $build = array(
    'object_log' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#attributes' => array('id' => 'object_log'),
      '#empty' => t('No objects stored.'),
    ),
    'object_log_pager' => array(
      '#theme' => 'pager',
    ),
  );
  
  return $build;
}

/**
 * Menu callback function for "admin/reports/object_log/$cid".
 *
 * This function will also look at the given query path to find any additional
 * objects that may have been requested.
 *
 * @param $cid
 *   The CID of an object to inspect.
 */
function object_log_object_page($cid) {
  // Determine all of the requested CIDs.
  if (strpos($_GET['q'], OBJECT_LOG_ADMIN_PATH) === 0) {
    $cids = explode('/', str_replace(OBJECT_LOG_ADMIN_PATH . '/', '', $_GET['q']));
  }
  else {
    $cids = array($cid);
  }
  $build = object_log_build_objects($cids);
  
  // Display the log table so the user can select another object to compare to
  // the given object.
  $build['object_log'] = array(
    'header' => array(
      '#type' => 'markup',
      '#markup' => t('Select an object to compare with @cid', array('@cid' => $cid)),
      '#prefix' => '<h2>',
      '#suffix' => '</h2>',
    ),
  );
  $build['object_log'] += object_log_build_log($cid);

  return $build;
}

/**
 * Display stored objects for inspection.
 *
 * @param $cids
 *   An array of CIDs representing objects to be displayed.
 * @return
 *   A render array of objects ready for inspection.
 */
function object_log_build_objects($cids = array()) {
  $build = array();
  $build['objects'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'object_log_objects',
      'class' => array('object-log', 'object-count-' . count($cids), 'clearfix'),
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'object_log') . '/object_log.admin.css'),
    ),
  );

  foreach ($cids as $delta => $cid) {
    if ($object = object_log_build_object($cid)) {
      $build['objects'][$delta] = $object;
    }
    else {
      $markup = '<div class="object"><h4 class="object-cid">' . check_plain($cid) . '</h3><p>' . t('Not in log.') . '</p></div>';
      $build['objects'][$delta] = array('#type' => 'markup', '#markup' => $markup);
    }
  }
  return $build;
}

/**
 * Build a render array for a stored object.
 *
 * @param $cid
 *   The CID of the object to display.
 * @return
 *   A render array for the object.
 */
function object_log_build_object($cid) {
  $cache = cache_get($cid, OBJECT_LOG_TABLE);
  if (!$cache) {
    return FALSE;
  }
  $object = $cache->data;
  $build = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('object_wrapper'),
    ),
  );
  $build['title'] = array(
    '#type' => 'markup',
    '#markup' => check_plain($cid),
    '#prefix' => '<h4 class="object-cid">',
    '#suffix' => '</h4>',
  );
  $build['object'] = array(
    '#type' => 'markup',
    '#markup' => kprint_r($object, TRUE, NULL),
    '#prefix' => '<div class="object">',
    '#suffix' => '</div>',
  );
  return $build;
}

/**
 * Form for clearing the object log.
 */
function object_log_clear_form($form, &$form_state) {
  $form = array();
  $form['clear'] = array(
    '#type' => 'submit', 
    '#value' => t('Clear object log'), 
    '#submit' => array('object_log_clear_form_submit'),
  );
  return $form;
}

/**
 * Submit callback for object_log_clear_form().
 */
function object_log_clear_form_submit($form, &$form_state) {
  cache_clear_all('*', OBJECT_LOG_TABLE, TRUE);
  drupal_set_message(t('Object log cleared.'));
  $form_state['redirect'] = 'admin/reports';
}
