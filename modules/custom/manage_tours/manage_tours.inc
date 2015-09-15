<?php

function manage_tours_main($form, &$form_state) {
    $form['search'] = array(
        '#type' => 'fieldset',
        '#title' => t('Tìm kiếm tour'),
        '#collapsible' => TRUE,
    );
    $form['search']['t_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Mã tour:'),
    );
    $form['search']['traveler_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên khách:'),
    );

    $rows = get_kot();
    $form['search']['kot'] = array(
        '#type' => 'select',
        '#title' => 'Loại tour:',
        '#options' => array('-All-') + $rows,
    );
    $form['search']['search_btn'] = array(
        '#type' => 'submit',
        '#value' => t('Tìm Kiếm'),
        '#ajax' => array(
            'callback' => 'ajax_search_button_callback',
        ),
    );
//    $form['search']['btn_export'] = array(
//        '#type' => 'submit',
//        '#value' => t('Export data to excel'),
//        '#executes_submit_callback' => TRUE,
//        '#submit' => array('log_excel'),
//    );
    $header = array(
        array('data' => t('Mã tour'), 'field' => 't.t_id'),
        array('data' => t('Tên khách'), 'field' => 't.traveler_name'),
        array('data' => t('Quốc tịch'), 'field' => 't.traveler_nation'),
        array('data' => t('Loại tour'), 'field' => 'kot.kot_name'),
        array('data' => t('Ngày'), 'field' => 't.t_date')
    );
    $rows_t = get_tours($form, $form_state, true);
    $_SESSION['rows'] = get_tours($form, $form_state);
    $form['list']['tour'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows_t,
        '#empty' => t('Hiện tại chưa có dữ liệu'),
        '#prefix' => '<div id="search_div">',
        '#suffix' => '</div>',
    );
    $form['pager_pager'] = array('#theme' => 'pager');
    return $form;
}

function get_tours($form, $form_state, $flag_limit = false) {
    $header = array(
        array('data' => t('Mã tour'), 'field' => 't.t_id'),
        array('data' => t('Tên khách'), 'field' => 't.traveler_name'),
        array('data' => t('Quốc tịch'), 'field' => 't.traveler_nation'),
        array('data' => t('Loại tour'), 'field' => 'kot.kot_name'),
        array('data' => t('Ngày'), 'field' => 't.t_date')
    );
    $query = db_select('sft_tours', 't')
            ->extend('TableSort');
    $query->fields('t', array('t_id', 'traveler_name', 'traveler_nation', 't_date'))
            ->fields('kot', array('kot_name'));
    $query->join('sft_kindoftours', 'kot', 'kot.kot_id=t.kot_id');
    $t_id = isset($form_state['values']['t_id']) ? $form_state['values']['t_id'] : '';
    $traveler_name = isset($form_state['values']['traveler_name']) ? $form_state['values']['traveler_name'] : '';
    $kot = isset($form_state['values']['kot']) ? $form_state['values']['kot'] : '';
    if ($t_id) {
        $query->condition('t_id', '%' . db_like($t_id) . '%', 'LIKE');
    }
    if ($traveler_name) {
        $query->condition('traveler_name', '%' . db_like($traveler_name) . '%', 'LIKE');
    }
    if ($kot) {
        $query->condition('t.kot_id', $kot);
    }
    $query->orderByHeader($header);
    $result = $query->execute();
    $rows = array();
    foreach ($result as $row) {
        $rows[$row->t_id] = array(
            l($row->t_id, 'manage/manage_tours/detail/' . $row->t_id),
            $row->traveler_name,
            $row->traveler_nation,
            $row->kot_name,
            $row->t_date,
        );
    }
    return $rows;
}

function ajax_search_button_callback($form, $form_state) {
    $commands = array();
    $rows = get_tours($form, $form_state, true);
    $_SESSION['rows'] = get_tours($form, $form_state);
    $form['list']['tour']['#rows'] = $rows;
    $commands[] = ajax_command_replace('#search_div', render($form['list']['tour']));
    return array('#type' => 'ajax', '#commands' => $commands);
}

function get_kot() {
    $q = db_select('sft_kindoftours', 'kot');
    $q->fields('kot', array('kot_id', 'kot_name'));
    $rs = $q->execute()->fetchAllKeyed(0, 1);
    return $rs;
}
