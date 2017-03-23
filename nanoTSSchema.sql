-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema nanoTS
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema nanoTS
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `nanoTS` DEFAULT CHARACTER SET utf8 ;
USE `nanoTS` ;

-- -----------------------------------------------------
-- Table `nanoTS`.`Projects`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nanoTS`.`Projects` (
  `ProjectID` VARCHAR(38) NOT NULL,
  `ProjectName` VARCHAR(45) NOT NULL,
  `ProjectPriority` VARCHAR(10) NOT NULL,
  `ProjectStatus` VARCHAR(10) NOT NULL,
  `ProjectType` VARCHAR(10) NOT NULL,
  PRIMARY KEY (`ProjectID`),
  UNIQUE INDEX `ProjectName_UNIQUE` (`ProjectName` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nanoTS`.`Calendars`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nanoTS`.`Calendars` (
  `CalendarID` VARCHAR(38) NOT NULL,
  `CalendarName` VARCHAR(45) NOT NULL,
  `CalendarYear` INT NOT NULL,
  PRIMARY KEY (`CalendarID`),
  UNIQUE INDEX `CalendarName_UNIQUE` (`CalendarName` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nanoTS`.`Employees`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nanoTS`.`Employees` (
  `EmployeeID` VARCHAR(38) NOT NULL,
  `EmployeeName` VARCHAR(45) NOT NULL,
  `Calendars_CalendarID` VARCHAR(38) NOT NULL,
  `EmployeeStart` DATE NOT NULL,
  `EmployeeStop` DATE NOT NULL,
  PRIMARY KEY (`EmployeeID`, `Calendars_CalendarID`),
  UNIQUE INDEX `EmployeeName_UNIQUE` (`EmployeeName` ASC),
  INDEX `fk_Employees_Calendars1_idx` (`Calendars_CalendarID` ASC),
  CONSTRAINT `fk_Employees_Calendars1`
    FOREIGN KEY (`Calendars_CalendarID`)
    REFERENCES `nanoTS`.`Calendars` (`CalendarID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nanoTS`.`TimeSheets`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nanoTS`.`TimeSheets` (
  `TimeSheetID` VARCHAR(38) NOT NULL,
  `TimeSheetStart` DATETIME NOT NULL,
  `TimeSheetStop` DATETIME NOT NULL,
  `TimeSheetComment` VARCHAR(80) NOT NULL,
  `Projects_ProjectID` VARCHAR(38) NOT NULL,
  `Employees_EmployeeID` VARCHAR(38) NOT NULL,
  PRIMARY KEY (`TimeSheetID`, `Projects_ProjectID`, `Employees_EmployeeID`),
  INDEX `fk_TimeSheets_Projects_idx` (`Projects_ProjectID` ASC),
  INDEX `fk_TimeSheets_Employees1_idx` (`Employees_EmployeeID` ASC),
  CONSTRAINT `fk_TimeSheets_Projects`
    FOREIGN KEY (`Projects_ProjectID`)
    REFERENCES `nanoTS`.`Projects` (`ProjectID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_TimeSheets_Employees1`
    FOREIGN KEY (`Employees_EmployeeID`)
    REFERENCES `nanoTS`.`Employees` (`EmployeeID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nanoTS`.`CalendarDetails`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nanoTS`.`CalendarDetails` (
  `CalendarDetailID` VARCHAR(38) NOT NULL,
  `CalendarDetailDate` DATETIME NOT NULL,
  `CalendarDetailType` VARCHAR(10) NOT NULL,
  `Calendars_CalendarID` VARCHAR(38) NOT NULL,
  PRIMARY KEY (`CalendarDetailID`, `Calendars_CalendarID`),
  INDEX `fk_CalendarDetails_Calendars1_idx` (`Calendars_CalendarID` ASC),
  CONSTRAINT `fk_CalendarDetails_Calendars1`
    FOREIGN KEY (`Calendars_CalendarID`)
    REFERENCES `nanoTS`.`Calendars` (`CalendarID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `nanoTS`.`Activities`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `nanoTS`.`Activities` (
  `ActivityID` VARCHAR(38) NOT NULL,
  `ActivityCode` VARCHAR(10) NOT NULL,
  `Projects_ProjectID` VARCHAR(38) NOT NULL,
  PRIMARY KEY (`ActivityID`, `Projects_ProjectID`),
  UNIQUE INDEX `ActivityCode_UNIQUE` (`ActivityCode` ASC),
  INDEX `fk_Activities_Projects1_idx` (`Projects_ProjectID` ASC),
  CONSTRAINT `fk_Activities_Projects1`
    FOREIGN KEY (`Projects_ProjectID`)
    REFERENCES `nanoTS`.`Projects` (`ProjectID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
