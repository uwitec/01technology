<?php

require('FLEA.php');

$dbDSN = array(
    'driver' => 'mysql',
    'host' => 'localhost',
    'login' => 'root',
    'password' => '',
    'database' => 'test'
);

FLEA::setAppInf('dbDSN', $dbDSN);
FLEA::setAppInf('internalCacheDir', 'D:/temp');

$acl = FLEA::getSingleton('FLEA_Acl_Manager');
/* @var $acl FLEA_Acl_Manager */

$user = $acl->getUserWithPermissions(array('username' => 'liaoyulei'));
dump($user);
