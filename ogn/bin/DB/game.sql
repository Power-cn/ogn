/*
Navicat MySQL Data Transfer

Source Server         : ogn
Source Server Version : 50717
Source Host           : localhost:3306
Source Database       : ogn

Target Server Type    : MYSQL
Target Server Version : 50717
File Encoding         : 65001

Date: 2017-06-09 20:22:49
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for account
-- ----------------------------
DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user` varchar(32) NOT NULL,
  `password` blob NOT NULL,
  `name` varchar(32) NOT NULL,
  `email` varchar(32) NOT NULL,
  `phone` varchar(32) NOT NULL,
  `createTime` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=643 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of account
-- ----------------------------
INSERT INTO `account` VALUES ('641', 'robot1', '', '', '', '', '0');

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `accountId` int(11) NOT NULL,
  `name` varchar(32) NOT NULL,
  `property` blob NOT NULL,
  `createTime` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=642 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('640', '641', 'robot1', 0x0000000B06000000020C0006726F626F74310600000004070000000206000000050700000001060000000606000000000600000007060000000006000000080A40A0000006000000090201060000000A0301060000000D0600000064060000000E06000000640C00046A736F6E0C004F7B0A202020226C617374686F737422203A20223132372E302E302E31222C0A202020226F66666C696E6522203A20313439363939363435302C0A202020227465737422203A202274657374220A7D0A, '0');
INSERT INTO `user` VALUES ('641', '642', 'robot1', '', '0');
