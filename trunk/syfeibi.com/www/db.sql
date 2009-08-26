/*
SQLyog Enterprise - MySQL GUI v7.02 
MySQL - 5.1.36-community-log : Database - syfeibi
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE DATABASE /*!32312 IF NOT EXISTS*/`syfeibi` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `syfeibi.com`;

/*Table structure for table `ws_admin_user` */

DROP TABLE IF EXISTS `ws_admin_user`;

CREATE TABLE `ws_admin_user` (
  `uid` int(8) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(64) DEFAULT NULL,
  `password` varchar(32) DEFAULT NULL,
  `delflg` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Data for the table `ws_admin_user` */

insert  into `ws_admin_user`(`uid`,`name`,`password`,`delflg`) values (1,'admin','admin',0);

/*Table structure for table `ws_category` */

DROP TABLE IF EXISTS `ws_category`;

CREATE TABLE `ws_category` (
  `cateid` int(8) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(64) NOT NULL,
  `delflg` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`cateid`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

/*Data for the table `ws_category` */

insert  into `ws_category`(`cateid`,`title`,`delflg`) values (1,'药品',0);

/*Table structure for table `ws_goods` */

DROP TABLE IF EXISTS `ws_goods`;

CREATE TABLE `ws_goods` (
  `goodsid` int(8) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(128) DEFAULT NULL,
  `summary` varchar(512) DEFAULT NULL,
  `detail` text,
  `cateid` int(8) DEFAULT '0',
  `image` varchar(64) DEFAULT NULL,
  `thums` varchar(64) DEFAULT NULL,
  `created` datetime NOT NULL,
  `updated` datetime DEFAULT NULL,
  `viewed` int(8) NOT NULL DEFAULT '0',
  `delflg` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`goodsid`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

/*Data for the table `ws_goods` */

insert  into `ws_goods`(`goodsid`,`name`,`summary`,`detail`,`cateid`,`image`,`thums`,`created`,`updated`,`viewed`,`delflg`) values (1,'柴芩清宁胶囊','清热解毒，和解表面。用于发热恶寒、咽痛流浊涕等上呼吸道感染之邪在肺卫证','<p>【药品名称】<br />\r\n&nbsp;通用名称：柴芩清宁胶囊<br />\r\n&nbsp;汉语拼音：Chaiqin Qingning Jiaonang<br />\r\n【成&nbsp;&nbsp;&nbsp; 份】黄芩苷、柴胡、人工牛黄。<br />\r\n【功能主治】清热解毒，和解表面。用于发热恶寒、咽痛流浊涕等上呼吸道感染之邪在肺卫证。<br />\r\n【规&nbsp;&nbsp;&nbsp; 格】每粒装0.3g<br />\r\n【用法用量】口服。一次3粒，一日3次。<br />\r\n【不良反应】II期临床期间，试验组出现1病例血肌酐疗后异常升高（疗前80.9umo1/L，疗后264.1 umo1/L)，两天后复查正常，目前尚无法判断该事件与药物的因果关系。<br />\r\n【禁&nbsp;&nbsp;&nbsp; 忌】对本品过敏者慎用。<br />\r\n【注意事项】<br />\r\n1.本品尚无用于妊娠及哺乳期妇女有效性和安全性研究数据。<br />\r\n2.本品尚无用于体温&gt;39C可WEC&gt;12X10L人群的有效性和安全性研究数据。<br />\r\n3.肝肾功能不全者慎用。<br />\r\n4.服药期间忌烟及辛辣、生冷、油腻食物。<br />\r\n5.不宜在服药期间同时服用滋补类中药。<br />\r\n6.过敏体质者慎用。<br />\r\n7.药品性状发生改变禁用。<br />\r\n【临床试验】本品于2003年经国家食品药品监管理局批准进行过499例临床试验。该实验纳入上呼吸道感染者，给予柴芩清宁胶囊一次3粒，一日3次。III期临床研究结果显示试验组疾病痊愈率为45.34%，中医证候痊愈率为44.05%，与对照组比较均有统计学意义。但II期临床研究期间，试验组有1例血肌酐疗后异常升高，目前尚无法判断与药物的因果关系。另外，有个别患者了现用药后腹胀等胃部不适。<br />\r\n【药理毒理】药效学试验结果表明，本品可减少氨水引咳小鼠的咳嗽次数；增加小鼠气管酚红排出量；抑制角叉菜胶致大鼠足跖肿胀；对角叉菜胶致大鼠发热和大肠杆菌致家兔发热有一定抑制作用；能抑制醋酸所致小鼠扭体反应和提高小鼠热板痛阈值；可使磷酰胺致免疫低下小鼠的血清溶血素水平升高。大鼠长期毒性试验结果表明，本品8.5、17g/Kg（以生药量计）灌胃给药连续4周，对大鼠体重增长和进食量有抑制作用，并可使用血清尿素氮和肌酐水平升高。<br />\r\n【贮&nbsp;&nbsp;&nbsp; 藏】密封，防潮<br />\r\n【包&nbsp;&nbsp;&nbsp; 装】铝塑泡罩包装。12粒/板 X 2板/盒<br />\r\n【有 效 期】24个月<br />\r\n【执行标准】国家食品药品监督管理局标准YBZ01082009<br />\r\n【批准文号】国药准字Z20090081<br />\r\n【生产企业】<br />\r\n企业名称：哈尔滨东方制药有限公司<br />\r\n生产地址：哈尔滨市尚志经济开发区迎宾路1号<br />\r\n邮政编码：150600<br />\r\n电话号码：0451-53335701 56769888<br />\r\n传真号码：0451-53335026<br />\r\n&nbsp;</p>',1,'afc26930e8cc48e571e1cd2e68f62405.jpg',NULL,'2009-07-13 13:09:33','2009-07-28 06:35:17',0,0),(2,'注射用甲磺酸二氢麦角碱 思清','急性缺血性脑卒中\r\n卒中后状态或脑外伤后遗症\r\n外周血管病（血管栓塞性脉管炎、雷诺氏病、动脉硬化和糖尿病引起的功能紊乱）','<p>【药品名称】<br />\r\n通用名：注射用甲磺酸二氢麦角碱<br />\r\n商品名：思清<br />\r\n英文名：Ergoloid Mesylate for Injection<br />\r\n汉语拼音：Zhushenyong Jiahuangsuan Erqingmaijiaojian<br />\r\n【性&nbsp;&nbsp;&nbsp; 状】<br />\r\n本品为白色或类白色疏松块状物。<br />\r\n【药理毒理】<br />\r\n据文献报道：在人体的作用机理还未被充分解释。在动物研究上，已经证明甲磺酸二氢麦角碱的作用是基于增加神经节细胞的代谢活性。通过降低无氧代谢和改进氧的利用改进脑组织的代谢能量。特别重要的是它在老年大脑中的神经化学过程的作用。甲磺酸二氢麦角碱影响卷入糖酵解主要酶的活性。因此，改进葡萄糖在大脑中的消耗。进而，功能性损伤神经的细胞内代谢得以改进。越来越多的数据可能建议甲磺酸二氢麦角碱作用在介入乙酰胆碱合成的胆碱乙酰化酶。此酶的活性在甲磺酸二氢麦角碱治疗几星期后被恢复。因此，本品用于识别功能障碍的病人是具有理论基础的。甲磺酸二氢麦角碱也作用在另一些大脑的中枢神经递质：多巴胺，5-羟色胺，去甲肾上腺素。甲磺酸二氢麦角碱作用在多巴胺和5-羟色胺受体上并为&alpha;-甲肾上腺素受体拮抗剂。最重要的甲磺酸二氢麦角碱的外周作用是阻断&alpha;-甲肾上腺素和通过5-羟色胺刺激引起的抑制血管收缩的外周多巴胺受体抑制，以及导致血压下降。<br />\r\n甲磺酸二氢麦角碱的中枢和外周作用的最终结果是改善脑部和外周的血流。<br />\r\n二氢麦角碱为相对无毒的物质，二氢麦角碱试验显示妊娠大鼠在服用12mg/kg后体重的增加无明显抑制。被报道的动物耐受的最大口服剂量为3到6mg/kg。<br />\r\n【药代动力学】<br />\r\n据文献报道，在6个成人年龄在76到86之间肌肉注射后，6个病人年龄在66到86之间口服后的要带动力学特性，比较平均年龄在25岁的健康受试者，<br />\r\n&nbsp;研究显示如下所述：<br />\r\n&nbsp;肾清除相当可观的降低 （50%）&nbsp;<br />\r\n&nbsp;代谢清除降氏（30％），可能由干与年龄有关的肝脏血流降低。<br />\r\n&nbsp;与口服后生物等效性相比具有可观的增加，随着吸收增加或通过肝脏的首过效应的降低。<br />\r\n&nbsp;甲磺酸二氢麦角碱从体内大部分由粪便排除，只有8％在尿中。清除分两个阶段：&alpha;阶段（包括各种体内分布阶段），&beta;阶段和排除。<br />\r\n【适 应 症】 <br />\r\n&nbsp;-急性缺血性脑卒中<br />\r\n&nbsp;-卒中后状态或脑外伤后遗症<br />\r\n&nbsp;-外周血管病（血管栓塞性脉管炎、雷诺氏病、动脉硬化和糖尿病引起的功能紊乱）<br />\r\n【用法用量】 <br />\r\n&nbsp;用注射用水溶解后．静脉滴注．肌内注射或皮下推注。<br />\r\n&nbsp;常用量O。3mg-0.6mg ，或遵医嘱。<br />\r\n【不良反应】 <br />\r\n&nbsp;通常耐受性良好，偶见消化道紊乱：恶心、呕吐、腹胀、厌食；个别病人出现视物模糊、鼻充血及皮疹。出现以上现象无须停药。头晕、心动过级、直立低血压和功能亢进偶有发生。<br />\r\n【禁 忌 症】 <br />\r\n&nbsp;对本品或本品中任何成分过敏者禁用；明显的心搏博徐缓及严重的低血压患者禁用。应避免用于严重肝肾功能障碍的病人、卟啉症和情神病患者。<br />\r\n【注意事项】<br />\r\n&nbsp;对于进行性局部缺血和严重外周血管病患者须小心使用。使用本品期间，如出现任何不良事件/或不良反应，请咨询医生。同时使用其他药品，请告知医生。请放置于儿童不能够触及的地方。 <br />\r\n【 孕妇及哺乳期妇女用药】<br />\r\n&nbsp;孕妇和哺乳期妇女用药的安全有效性尚不明确。<br />\r\n【儿童用药】<br />\r\n&nbsp;尚缺乏本品儿童用药的安全有效性研究资料。<br />\r\n【老年患者用药】<br />\r\n&nbsp;参见【用法用量】，或通医嘱。<br />\r\n【药物相互作用】 <br />\r\n&nbsp;与抗高血压药合用可能增加降压作用。大剂量甲磺酸二氢麦角碱和另一些麦角生物碱以及舒马曲坦不能合并使用。与环孢素合用时，将改变环孢霉素的药代动力学。多巴胺与二氢麦角碱联台应用时，可诱导周围血管痉挛，特别是肢体远端血管收缩。请告诉医生你使用的所有药物。<br />\r\n【药物过量】<br />\r\n&nbsp;大量服用甲磺酸二氢麦角碱可出现急性中毒，包括：脑血管和冠状血管供血不足的低血压、呕吐和腹泻。严重时有血管痉挛、惊厥和意识缺损可能发生。<br />\r\n【规&nbsp;&nbsp;&nbsp; 格】 <br />\r\n&nbsp;0.3mg<br />\r\n【贮&nbsp;&nbsp;&nbsp; 藏】<br />\r\n&nbsp;在阴凉干烽处保存<br />\r\n【有 效 期】<br />\r\n&nbsp;暂定二年<br />\r\n【包&nbsp;&nbsp;&nbsp; 装】 <br />\r\n&nbsp;每盒1 支、每盒2 支<br />\r\n【批准文号】 <br />\r\n【生产企业】 <br />\r\n&nbsp;黑龙江哈尔滨医大药业有限公司制造<br />\r\n&nbsp;万维医药有限公司总经销<br />\r\n&nbsp;地址：辽宁省沈阳市今天往13 帕号格林自由成日座2705 室电话：024 一阅522088 <br />\r\n经典脑代谢增强药物美国FOA 确认唯一的用于治疗老年认识障碍药物增强脑神经细胞能量代谢和微循环，用于治疗急性缺血性脑卒中，脑外临后遗症及外周血管病<br />\r\n甲磺酸二氢麦角碱冻干粉针Ergoloid Mesylate for In 』 ection <br />\r\n黑龙江哈尔滨医大药业有限公司制造辽宁省万维医药有限公司总经销</p>\r\n<p>&nbsp;</p>',1,'9b3fc5753eb915e9231690e77babe16a.jpg',NULL,'2009-07-13 13:09:40','2009-07-28 06:35:48',0,0);

/*Table structure for table `ws_message` */

DROP TABLE IF EXISTS `ws_message`;

CREATE TABLE `ws_message` (
  `msgid` int(8) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(64) NOT NULL,
  `content` text,
  `created` datetime NOT NULL,
  `user` varchar(64) DEFAULT NULL,
  `email` varchar(64) DEFAULT NULL,
  `tel` varchar(64) DEFAULT NULL,
  `viewed` tinyint(1) NOT NULL DEFAULT '0',
  `delflg` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`msgid`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

/*Data for the table `ws_message` */

insert  into `ws_message`(`msgid`,`title`,`content`,`created`,`user`,`email`,`tel`,`viewed`,`delflg`) values (1,'aaaa','aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa','2009-07-12 22:28:45',NULL,NULL,'838652',1,0),(2,'bbbbb','bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb','2009-07-12 22:49:11',NULL,'allan8212@sohu.com',NULL,1,0),(3,'dsdfds','dsfsdf','2009-07-28 06:30:27','sdf','sdfs@3432.com','34343',1,0);

/*Table structure for table `ws_news` */

DROP TABLE IF EXISTS `ws_news`;

CREATE TABLE `ws_news` (
  `newsid` int(8) unsigned NOT NULL AUTO_INCREMENT,
  `sort` int(8) NOT NULL DEFAULT '0',
  `title` varchar(64) NOT NULL,
  `content` text,
  `created` datetime NOT NULL,
  `updated` datetime DEFAULT NULL,
  `viewed` int(8) DEFAULT '0',
  `author` varchar(32) NOT NULL,
  `type` int(1) NOT NULL DEFAULT '0' COMMENT '0,normal。1，notice',
  `delflg` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`newsid`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;

/*Data for the table `ws_news` */

insert  into `ws_news`(`newsid`,`sort`,`title`,`content`,`created`,`updated`,`viewed`,`author`,`type`,`delflg`) values (1,0,'ttttt','tttttttttttttttttttttttttttttttttttttttttttttttttt','2009-07-09 14:27:00','2009-07-11 00:35:23',0,'admin',0,0),(2,3,'asdfasdf','<p>asdfasdfasdf</p>','2009-07-12 01:12:49','2009-07-12 01:17:16',0,'admin',1,0),(3,45,'hrthrthrtsdfasdfasdfdd33333','<p>hrthrthrthrthrtasdfaasdfasdfaasdfasdfasdfasdf</p>','2009-07-12 01:15:44','2009-07-18 16:17:15',0,'admin',0,0);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
