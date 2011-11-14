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

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;