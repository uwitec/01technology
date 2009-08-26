-- phpMyAdmin SQL Dump
-- version 2.9.1.1
-- http://www.phpmyadmin.net
-- 
-- 主机: localhost
-- 生成日期: 2007 年 07 月 05 日 18:05
-- 服务器版本: 5.0.21
-- PHP 版本: 5.2.3
-- 
-- 数据库: `test`
-- 

-- --------------------------------------------------------

-- 
-- 表的结构 `permissions`
-- 

CREATE TABLE `permissions` (
  `permission_id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(32) NOT NULL,
  `access_data` text,
  PRIMARY KEY  (`permission_id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

-- 
-- 表的结构 `roles`
-- 

CREATE TABLE `roles` (
  `role_id` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(32) NOT NULL,
  PRIMARY KEY  (`role_id`),
  UNIQUE KEY `rolename` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

-- 
-- 表的结构 `roles_has_permissions`
-- 

CREATE TABLE `roles_has_permissions` (
  `role_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY  (`role_id`,`permission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

-- 
-- 表的结构 `user_groups`
-- 

CREATE TABLE `user_groups` (
  `user_group_id` int(10) unsigned NOT NULL auto_increment,
  `parent_id` int(10) NOT NULL,
  `name` varchar(32) NOT NULL,
  `description` varchar(240) default NULL,
  `created` int(10) unsigned NOT NULL,
  `left_value` int(10) unsigned NOT NULL,
  `right_value` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`user_group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

-- 
-- 表的结构 `user_groups_has_permissions`
-- 

CREATE TABLE `user_groups_has_permissions` (
  `user_group_id` int(10) unsigned NOT NULL,
  `permission_id` int(10) unsigned NOT NULL,
  `is_allow` tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (`user_group_id`,`permission_id`,`is_allow`),
  KEY `permission_id` (`permission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

-- 
-- 表的结构 `user_groups_has_roles`
-- 

CREATE TABLE `user_groups_has_roles` (
  `user_group_id` int(10) unsigned NOT NULL,
  `role_id` int(10) unsigned NOT NULL,
  `is_include` tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (`user_group_id`,`role_id`,`is_include`),
  KEY `role_id` (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

-- 
-- 表的结构 `users`
-- 

CREATE TABLE `users` (
  `user_id` int(10) unsigned NOT NULL auto_increment,
  `user_group_id` int(10) unsigned NOT NULL,
  `username` varchar(32) NOT NULL,
  `password` varchar(64) NOT NULL,
  `email` varchar(128) NOT NULL,
  `created` int(10) unsigned NOT NULL,
  `updated` int(10) unsigned NOT NULL,
  PRIMARY KEY  (`user_id`),
  KEY `user_group_id` (`user_group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

-- 
-- 表的结构 `users_has_permissions`
-- 

CREATE TABLE `users_has_permissions` (
  `user_id` int(10) unsigned NOT NULL,
  `permission_id` int(10) unsigned NOT NULL,
  `is_allow` tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (`user_id`,`permission_id`,`is_allow`),
  KEY `permission_id` (`permission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

-- 
-- 表的结构 `users_has_roles`
-- 

CREATE TABLE `users_has_roles` (
  `user_id` int(10) unsigned NOT NULL,
  `role_id` int(10) unsigned NOT NULL,
  `is_include` tinyint(1) NOT NULL default '1',
  PRIMARY KEY  (`user_id`,`role_id`,`is_include`),
  KEY `role_id` (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 
-- 限制导出的表
-- 

-- 
-- 限制表 `user_groups_has_permissions`
-- 
ALTER TABLE `user_groups_has_permissions`
  ADD CONSTRAINT `user_groups_has_permissions_ibfk_1` FOREIGN KEY (`user_group_id`) REFERENCES `user_groups` (`user_group_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `user_groups_has_permissions_ibfk_2` FOREIGN KEY (`permission_id`) REFERENCES `permissions` (`permission_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

-- 
-- 限制表 `user_groups_has_roles`
-- 
ALTER TABLE `user_groups_has_roles`
  ADD CONSTRAINT `user_groups_has_roles_ibfk_1` FOREIGN KEY (`user_group_id`) REFERENCES `user_groups` (`user_group_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `user_groups_has_roles_ibfk_2` FOREIGN KEY (`role_id`) REFERENCES `roles` (`role_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

-- 
-- 限制表 `users`
-- 
ALTER TABLE `users`
  ADD CONSTRAINT `users_ibfk_1` FOREIGN KEY (`user_group_id`) REFERENCES `user_groups` (`user_group_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

-- 
-- 限制表 `users_has_permissions`
-- 
ALTER TABLE `users_has_permissions`
  ADD CONSTRAINT `users_has_permissions_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `users_has_permissions_ibfk_2` FOREIGN KEY (`permission_id`) REFERENCES `permissions` (`permission_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

-- 
-- 限制表 `users_has_roles`
-- 
ALTER TABLE `users_has_roles`
  ADD CONSTRAINT `users_has_roles_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `users_has_roles_ibfk_2` FOREIGN KEY (`role_id`) REFERENCES `roles` (`role_id`) ON DELETE NO ACTION ON UPDATE NO ACTION;
