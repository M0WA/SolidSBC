SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

DROP SCHEMA IF EXISTS `SolidSBC` ;
CREATE SCHEMA IF NOT EXISTS `SolidSBC` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `SolidSBC` ;

-- -----------------------------------------------------
-- Table `SolidSBC`.`Clients`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`Clients` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`Clients` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `uuid` VARCHAR(512) CHARACTER SET 'utf8' COLLATE 'utf8_general_ci' NOT NULL ,
  `name` VARCHAR(512) CHARACTER SET 'utf8' COLLATE 'utf8_general_ci' NULL ,
  PRIMARY KEY (`ID`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


-- -----------------------------------------------------
-- Table `SolidSBC`.`Configs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`Configs` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`Configs` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `xmlConfig` VARCHAR(4096) NOT NULL ,
  PRIMARY KEY (`ID`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


-- -----------------------------------------------------
-- Table `SolidSBC`.`ClientsConfigs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`ClientsConfigs` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`ClientsConfigs` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `CONFIG_ID` INT NOT NULL ,
  `CLIENT_ID` INT NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_CLIENTCONFIGS_CLIENT_ID` (`CLIENT_ID` ASC) ,
  INDEX `FK_CLIENTCONFIGS_CONFIG_ID` (`CONFIG_ID` ASC) ,
  CONSTRAINT `FK_CLIENTCONFIGS_CLIENT_ID`
    FOREIGN KEY (`CLIENT_ID` )
    REFERENCES `SolidSBC`.`Clients` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `FK_CLIENTCONFIGS_CONFIG_ID`
    FOREIGN KEY (`CONFIG_ID` )
    REFERENCES `SolidSBC`.`Configs` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `SolidSBC`.`ClientHistory`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`ClientHistory` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`ClientHistory` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `CLIENT_ID` INT NOT NULL ,
  `action` INT UNSIGNED NOT NULL ,
  `actiontime` DATETIME NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_CLIENTHISTORY_CLIENT_ID` (`CLIENT_ID` ASC) ,
  CONSTRAINT `FK_CLIENTHISTORY_CLIENT_ID`
    FOREIGN KEY (`CLIENT_ID` )
    REFERENCES `SolidSBC`.`Clients` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


-- -----------------------------------------------------
-- Table `SolidSBC`.`testLib_HDResults`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`testLib_HDResults` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`testLib_HDResults` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `CLIENT_ID` INT NOT NULL ,
  `duration` DOUBLE NOT NULL ,
  `amountbytes` BIGINT NOT NULL ,
  `wait` BIGINT NOT NULL ,
  `resulttype` INT NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_TESTLIB_HDRESULT_CLIENT_ID` (`CLIENT_ID` ASC) ,
  CONSTRAINT `FK_TESTLIB_HDRESULT_CLIENT_ID`
    FOREIGN KEY (`CLIENT_ID` )
    REFERENCES `SolidSBC`.`Clients` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


-- -----------------------------------------------------
-- Table `SolidSBC`.`testLib_CPUMeasureResults`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`testLib_CPUMeasureResults` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`testLib_CPUMeasureResults` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `CLIENT_ID` INT NOT NULL ,
  `addduration` DOUBLE NOT NULL ,
  `divduration` DOUBLE NOT NULL ,
  `overallduration` DOUBLE NOT NULL ,
  `addmultiplier` BIGINT NOT NULL ,
  `divmultiplier` BIGINT NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_TESTLIB_CPUMEASUREREULT_CLIENT_ID` (`CLIENT_ID` ASC) ,
  CONSTRAINT `FK_TESTLIB_CPUMEASUREREULT_CLIENT_ID`
    FOREIGN KEY (`CLIENT_ID` )
    REFERENCES `SolidSBC`.`Clients` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


-- -----------------------------------------------------
-- Table `SolidSBC`.`testLib_NetPingResults`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`testLib_NetPingResults` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`testLib_NetPingResults` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `CLIENT_ID` INT NOT NULL ,
  `rtt_duration` BIGINT NOT NULL ,
  `icmp_opts` BIGINT NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_TESTLIB_PINGRESULT_CLIENT_ID` (`CLIENT_ID` ASC) ,
  CONSTRAINT `FK_TESTLIB_PINGRESULT_CLIENT_ID`
    FOREIGN KEY (`CLIENT_ID` )
    REFERENCES `SolidSBC`.`Clients` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


-- -----------------------------------------------------
-- Table `SolidSBC`.`testLib_NetTCPResults`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`testLib_NetTCPResults` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`testLib_NetTCPResults` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `CLIENT_ID` INT NOT NULL ,
  `handshake_duration` DOUBLE NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_TESTLIB_NETTCPRESULT_CLIENT_ID` (`CLIENT_ID` ASC) ,
  CONSTRAINT `FK_TESTLIB_NETTCPRESULT_CLIENT_ID`
    FOREIGN KEY (`CLIENT_ID` )
    REFERENCES `SolidSBC`.`Clients` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


-- -----------------------------------------------------
-- Table `SolidSBC`.`testLib_MemoryResults`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `SolidSBC`.`testLib_MemoryResults` ;

CREATE  TABLE IF NOT EXISTS `SolidSBC`.`testLib_MemoryResults` (
  `ID` INT NOT NULL AUTO_INCREMENT ,
  `CLIENT_ID` INT NOT NULL ,
  `malloczeroduration` DOUBLE NOT NULL ,
  `bytes` BIGINT NOT NULL ,
  PRIMARY KEY (`ID`) ,
  INDEX `FK_TESTLIB_MEMRESULT_CLIENT_ID` (`CLIENT_ID` ASC) ,
  CONSTRAINT `FK_TESTLIB_MEMRESULT_CLIENT_ID`
    FOREIGN KEY (`CLIENT_ID` )
    REFERENCES `SolidSBC`.`Clients` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci;


-- -----------------------------------------------------
-- Placeholder table for view `SolidSBC`.`ClientConfigInfo`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `SolidSBC`.`ClientConfigInfo` (`client id` INT, `client` INT, `xml config` INT);

-- -----------------------------------------------------
-- Placeholder table for view `SolidSBC`.`ClientHistoryInfo`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `SolidSBC`.`ClientHistoryInfo` (`ID` INT, `client id` INT, `client` INT, `action` INT, `datetime` INT);

-- -----------------------------------------------------
-- Placeholder table for view `SolidSBC`.`ClientsSessions`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `SolidSBC`.`ClientsSessions` (`start ID` INT, `end ID` INT, `CLIENT_ID` INT, `client` INT, `start time` INT, `end time` INT);

-- -----------------------------------------------------
-- function GetClientName
-- -----------------------------------------------------

USE `SolidSBC`;
DROP function IF EXISTS `SolidSBC`.`GetClientName`;

DELIMITER $$
USE `SolidSBC`$$
CREATE FUNCTION `SolidSBC`.`GetClientName` (
    clientID BIGINT
    ) RETURNS VARCHAR(1024)

BEGIN
    DECLARE `retval` VARCHAR(1024);
    
    SET `retval` = (
        SELECT 
        CASE
            WHEN `name` IS NOT NULL THEN `name`
            ELSE `uuid`
        END
        FROM `SolidSBC`.`Clients` WHERE `ID` = `clientID`
    );
    
    RETURN `retval`;
END
$$

DELIMITER ;

-- -----------------------------------------------------
-- View `SolidSBC`.`ClientConfigInfo`
-- -----------------------------------------------------
DROP VIEW IF EXISTS `SolidSBC`.`ClientConfigInfo` ;
DROP TABLE IF EXISTS `SolidSBC`.`ClientConfigInfo`;
USE `SolidSBC`;
CREATE  OR REPLACE VIEW `SolidSBC`.`ClientConfigInfo` AS 
SELECT
    `cc`.`CLIENT_ID` AS `client id`,
    
    GetClientName(`cc`.`CLIENT_ID`) AS `client`,
    
    `xmlConfig` AS `xml config`
FROM       `ClientsConfigs` AS `cc`
INNER JOIN `Clients` AS `cli` ON ( `cli`.`ID` = `cc`.`CLIENT_ID` )
INNER JOIN `Configs` AS `cfg` ON ( `cfg`.`ID` = `cc`.`CONFIG_ID` )
GROUP BY `cc`.`CLIENT_ID`;

-- -----------------------------------------------------
-- View `SolidSBC`.`ClientHistoryInfo`
-- -----------------------------------------------------
DROP VIEW IF EXISTS `SolidSBC`.`ClientHistoryInfo` ;
DROP TABLE IF EXISTS `SolidSBC`.`ClientHistoryInfo`;
USE `SolidSBC`;
CREATE  OR REPLACE VIEW `SolidSBC`.`ClientHistoryInfo` AS
SELECT
    `ch`.`ID`,

    `c`.`ID` AS `client id`,

    GetClientName(`c`.`ID`) AS `client`,
    
    CASE 
        WHEN `ch`.`action` = 1 THEN 'login'
        WHEN `ch`.`action` = 2 THEN 'logout'
        ELSE 'n/a'
    END AS `action`,
    
    `ch`.`actiontime` AS `datetime`
    
FROM `ClientHistory` AS `ch`
INNER JOIN `Clients` AS `c`  ON ( `ch`.`CLIENT_ID` = `c`.`ID` )
ORDER BY `ch`.`CLIENT_ID`;

-- -----------------------------------------------------
-- View `SolidSBC`.`ClientsSessions`
-- -----------------------------------------------------
DROP VIEW IF EXISTS `SolidSBC`.`ClientsSessions` ;
DROP TABLE IF EXISTS `SolidSBC`.`ClientsSessions`;
USE `SolidSBC`;
CREATE  OR REPLACE VIEW `SolidSBC`.`ClientsSessions` AS
SELECT

    `ch_start`.`ID` AS `start ID`,
    
    IF (
        `ch_end`.`ID` IS NOT NULL,
        `ch_end`.`ID`,
        IF (
            `ch_end_last`.`ID` IS NOT NULL,
            `ch_end_last`.`ID`,
            ''
        )
    ) AS `end ID`,


    `ch_start`.`CLIENT_ID`                AS `CLIENT_ID`,
    GetClientName(`ch_start`.`CLIENT_ID`) AS `client`,
    
    `ch_start`.`actiontime` AS `start time`,
        
    IF (
        `ch_end`.`actiontime` IS NOT NULL,
        `ch_end`.`actiontime`,
        IF (
            `ch_end_last`.`actiontime` IS NOT NULL,
            `ch_end_last`.`actiontime`,
            'active'
        )
    )
    AS `end time`
    
FROM `SolidSBC`.`ClientHistory` AS `ch_start`
LEFT JOIN `SolidSBC`.`ClientHistory` AS `ch_next`     ON ( `ch_next`.`CLIENT_ID`     = `ch_start`.`CLIENT_ID` AND `ch_start`.`ID` < `ch_next`.`ID`     AND `ch_next`.`action` = 1 )
LEFT JOIN `SolidSBC`.`ClientHistory` AS `ch_end`      ON ( `ch_end`.`CLIENT_ID`      = `ch_start`.`CLIENT_ID` AND `ch_start`.`ID` < `ch_end`.`ID`      AND `ch_end`.`ID` < `ch_next`.`ID` AND `ch_end`.`action` = 2)
LEFT JOIN `SolidSBC`.`ClientHistory` AS `ch_end_last` ON ( `ch_end_last`.`CLIENT_ID` = `ch_start`.`CLIENT_ID` AND `ch_start`.`ID` < `ch_end_last`.`ID` AND `ch_end_last`.`action` = 2 AND (`ch_next`.`ID` > `ch_end_last`.`ID` OR (`ch_next`.`ID` IS NULL) ))
WHERE `ch_start`.`action` = 1;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
