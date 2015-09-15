<?php

function manage_tours_detail($form, &$form_state, $t_id) {
    $form = array();
    $info = getinfo_tour($t_id);
    $form['traveler_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên khách'),
        '#value' => $info['traveler_name'],
        '#disabled' => true
    );
    $form['traveler_nation'] = array(
        '#type' => 'textfield',
        '#title' => t('Quốc tịch'),
        '#value' => $info['traveler_nation'],
        '#disabled' => true
    );
    $form['kot_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Loại tour'),
        '#value' => $info['kot_name'],
        '#disabled' => true
    );
    $form['t_date'] = array(
        '#type' => 'textfield',
        '#title' => t('Ngày đặt'),
        '#value' => $info['t_date'],
        '#disabled' => true
    );

    $form['traveler_address'] = array(
        '#type' => 'textfield',
        '#title' => t('Địa chỉ'),
        '#value' => $info['traveler_address'],
        '#disabled' => true
    );
    $form['traveler_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'),
        '#value' => $info['traveler_email'],
        '#disabled' => true
    );
    $form['traveler_message'] = array(
        '#type' => 'textarea',
        '#title' => t('Lời nhắn'),
        '#value' => $info['traveler_message'],
        '#disabled' => true
    );
    return $form;
}

function getinfo_tour($t_id) { //Trả về thông tin của loại tài sản theo $maloaitaisan
    $q = db_select('sft_tours', 't');
    $q->fields('t', array('traveler_name', 'traveler_nation', 't_date', 'traveler_address', 'traveler_email', 'traveler_message'))
            ->fields('kot', array('kot_name'));
    $q->join('sft_kindoftours', 'kot', 'kot.kot_id=t.kot_id');
    $q->condition('t.t_id', $t_id);
    $rs = $q->execute()->fetchAssoc();
    return $rs;
}
