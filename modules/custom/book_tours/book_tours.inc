<?php

function book_tours_main($form, &$form_state) {
    require_once DRUPAL_ROOT . '/includes/locale.inc';
    $form = array();
//    if (is_numeric('dasdasds') == true) {
//        die('true');
//    } else {
//        die('fall');
//    }
    $form['traveler_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Your Name'),
        '#required' => TRUE,
    );
    $form['traveler_nation'] = array(
        '#type' => 'select',
        '#title' => t('Nation'),
        '#options' => country_get_list(),
        '#required' => TRUE,
    );
    $form['kot_id'] = array(
        '#type' => 'select',
        '#title' => t('Kind of Tours'),
        '#options' => get_kot(),
        '#required' => TRUE,
    );
    $form['t_date'] = array(
        '#type' => 'date_popup',
        // '#title' => t('Date of tour'),
        '#date_format' => 'Y-m-d H:i:s',
        '#required' => TRUE,
    );

    $form['traveler_amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Amount of tourist'),
        '#required' => TRUE,
    );
    $form['traveler_address'] = array(
        '#type' => 'textfield',
        '#title' => t('Address of hotel'),
        '#required' => TRUE,
    );
    $form['traveler_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#required' => TRUE,
    );
    $form['traveler_message'] = array(
        '#type' => 'textarea',
        '#title' => t('Message'),
    );
    $form['btn_them'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#validate' => array('tour_add_validate'),
        '#submit' => array('tour_add'),
        '#attributes' => array('onclick' => 'return confirm(\'Do you want to book this tour?\');', 'title' => 'Confirm'),
    );

    return $form;
}

function tour_add($form, &$form_state) {
    $t_id = uniqid('SFT', true);
    $traveler_name = isset($form_state['values']['traveler_name']) ? $form_state['values']['traveler_name'] : "";
    $traveler_nation = $form['traveler_nation']['#options'][$form_state['values']['traveler_nation']];
    $kot_id = isset($form_state['values']['kot_id']) ? $form_state['values']['kot_id'] : key(qlts_quanlyham_get_loaitaisan());
    $t_date = isset($form_state['values']['t_date']) ? $form_state['values']['t_date'] : "";
    $traveler_address = isset($form_state['values']['traveler_address']) ? $form_state['values']['traveler_address'] : "";
    $traveler_amount = isset($form_state['values']['traveler_amount']) ? intval($form_state['values']['traveler_amount']) : 1;
    $traveler_email = isset($form_state['values']['traveler_email']) ? $form_state['values']['traveler_email'] : "";
    $traveler_message = isset($form_state['values']['traveler_message']) ? $form_state['values']['traveler_message'] : "";
    $data = array(
        't_id' => $t_id,
        'traveler_name' => $traveler_name,
        'traveler_nation' => $traveler_nation,
        'kot_id' => $kot_id,
        't_date' => $t_date,
        'traveler_amount' => $traveler_amount,
        'traveler_address' => $traveler_address,
        'traveler_email' => $traveler_email,
        'traveler_message' => $traveler_message,
    );
    try {
        db_insert('sft_tours')->fields($data)->execute();
        drupal_set_message("$traveler_name booked this tour successfully!");
    } catch (Exception $exc) {
        drupal_set_message('Error (' . $exc->getMessage() . ')', 'error');
    }
}

function tour_add_validate($form, &$form_state) {
    if ($form_state['values']['traveler_name'] == "") {
        form_set_error('traveler_name', t('You must type your name!'));
    }
    if ($form_state['values']['traveler_nation'] == 'selected') {
        form_set_error('traveler_nation', t('You must select your nation!'));
    }
    if ($form_state['values']['kot_id'] == 'selected') {
        form_set_error('kot_id', t('You must select kind of tours!'));
    }
    if ($form_state['values']['t_date'] == "") {
        form_set_error('t_date', t('You must type date!'));
    }
    if (!is_numeric($form_state['values']['traveler_amount'])) {
        form_set_error('traveler_amount', t('You must type amount of traveler (number)!'));
    }
    if ($form_state['values']['traveler_address'] == "") {
        form_set_error('traveler_address', t('You must type your address!'));
    }
    if (!filter_var($form_state['values']['traveler_email'], FILTER_VALIDATE_EMAIL)) {
        form_set_error('traveler_email', t('You must type your email!'));
    }
}

function get_kot() {
    $q = db_select('sft_kindoftours', 'kot');
    $q->fields('kot', array('kot_id', 'kot_name'));
    $rs = $q->execute()->fetchAllKeyed(0, 1);
    return $rs;
}
