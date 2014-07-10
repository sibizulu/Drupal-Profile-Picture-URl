<?php
class picture_url_custom_handler extends views_handler_field_user_picture {
  
  function option_definition() {
    $options = parent::option_definition();
    $options['picture_url'] = array('default' => "", 'bool' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state){
    parent::options_form($form, $form_state);

    $form['picture_url'] = array(
      '#title' => t("Picture URL"),
      '#description' => t("Picture URL"),
      '#type' => 'checkbox',
      '#default_value' => $this->options['picture_url'],
    );

  }

  function render($values) { 
    if ($this->options['picture_url']) { 
    global $base_url;
    if ($picture_fid = $this->get_value($values)) {
      $picture = file_load($picture_fid);
      $output = str_replace('public://',$base_url.'/sites/default/files/',image_style_url($this->options['image_style'],$picture->uri));
    }
    else {
      $output = $base_url.'/misc/druplicon.png';;
    }
   } else {
    if ($this->options['image_style'] && module_exists('image')) {
      // @todo: Switch to always using theme('user_picture') when it starts
      // supporting image styles. See http://drupal.org/node/1021564
      if ($picture_fid = $this->get_value($values)) {
        $picture = file_load($picture_fid);
        $picture_filepath = $picture->uri;
      }
      else {
        $picture_filepath = variable_get('user_picture_default', '');
      }
      if (file_valid_uri($picture_filepath)) {
        $output = theme('image_style', array('style_name' => $this->options['image_style'], 'path' => $picture_filepath));
        if ($this->options['link_photo_to_profile'] && user_access('access user profiles')) {
          $uid = $this->get_value($values, 'uid');
          $output = l($output, "user/$uid", array('html' => TRUE));
        }
      }
      else {
        $output = '';
      }
    }
    else {
      // Fake an account object.
      $account = new stdClass();
      if ($this->options['link_photo_to_profile']) {
        // Prevent template_preprocess_user_picture from adding a link
        // by not setting the uid.
        $account->uid = $this->get_value($values, 'uid');
      }
      $account->name = $this->get_value($values, 'name');
      $account->mail = $this->get_value($values, 'mail');
      $account->picture = $this->get_value($values);
      $output = theme('user_picture', array('account' => $account));
    }
   }
   return $output;
  }
}

?>