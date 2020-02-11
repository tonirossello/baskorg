-- phpMyAdmin SQL Dump
-- version 4.9.4
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Feb 11, 2020 at 01:37 PM
-- Server version: 10.2.29-MariaDB
-- PHP Version: 7.2.5

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `cbsacabaneta`
--

DELIMITER $$
--
-- Procedures
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `nadiesocio` ()  BEGIN

Declare idj int(5);
Declare terminado int(1) default 0;
DECLARE cur1 Cursor for SELECT id from jugadors;
Declare continue Handler for not found set terminado=1;

open cur1; 
lp1: LOOP
FETCH cur1 into idj;
IF terminado=1 THEN 
	Leave lp1;
END IF; 
	UPDATE jugadors set Soci = "NO SOCI" where id=idj;
END LOOP lp1;
close cur1;

END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `activitats`
--

CREATE TABLE `activitats` (
  `id` int(5) UNSIGNED NOT NULL,
  `Nom` varchar(50) NOT NULL,
  `Descripcio` varchar(300) DEFAULT NULL,
  `preu_soci` decimal(6,2) NOT NULL,
  `preu_no_soci` decimal(6,2) NOT NULL,
  `preu_reserva` decimal(6,2) UNSIGNED DEFAULT NULL,
  `idtemporada` int(3) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- --------------------------------------------------------

--
-- Table structure for table `categoria`
--

CREATE TABLE `categoria` (
  `id` int(10) UNSIGNED NOT NULL,
  `Nom` varchar(255) NOT NULL,
  `Quota` int(3) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `categoria`
--

INSERT INTO `categoria` (`id`, `Nom`, `Quota`) VALUES
(1, 'Escoleta', 250),
(2, 'Iniciació', 320),
(3, 'Premini', 320),
(4, 'Mini', 320),
(5, 'Infantil', 320),
(6, 'Cadet', 320),
(7, 'Júnior', 400),
(8, 'Sénior', 450),
(9, 'Preinfantil', 320),
(10, 'Sub23', 450),
(11, 'Sense equip', 0);

-- --------------------------------------------------------

--
-- Table structure for table `clubs`
--

CREATE TABLE `clubs` (
  `id` int(8) NOT NULL,
  `nom` varchar(30) NOT NULL,
  `codi` varchar(8) NOT NULL,
  `propietari` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `clubs`
--

INSERT INTO `clubs` (`id`, `nom`, `codi`, `propietari`) VALUES
(1, 'Bàsquet Sa Cabaneta', 'cab12345', 5),
(2, 'ClubProva', '907907e4', 3);

--
-- Triggers `clubs`
--
DELIMITER $$
CREATE TRIGGER `before_insert_club` BEFORE INSERT ON `clubs` FOR EACH ROW BEGIN

DECLARE random VARCHAR(8);
SELECT LEFT(UUID(), 8) INTO random;

SET NEW.codi = random;

END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `comandes`
--

CREATE TABLE `comandes` (
  `id` int(10) UNSIGNED NOT NULL,
  `idjugador` int(5) UNSIGNED DEFAULT NULL,
  `data` datetime(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- --------------------------------------------------------

--
-- Table structure for table `cos tecnic`
--

CREATE TABLE `cos tecnic` (
  `idequip` int(5) UNSIGNED NOT NULL,
  `identrenador` int(3) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- --------------------------------------------------------

--
-- Table structure for table `entrenadors`
--

CREATE TABLE `entrenadors` (
  `id` int(3) UNSIGNED NOT NULL,
  `Nom` varchar(255) DEFAULT NULL,
  `Llicència` varchar(255) DEFAULT NULL,
  `Data Naixement` varchar(255) DEFAULT NULL,
  `Dni` varchar(255) DEFAULT NULL,
  `Tfn` double DEFAULT NULL,
  `Adreça` varchar(255) DEFAULT NULL,
  `Població` varchar(255) DEFAULT NULL,
  `Email` varchar(255) DEFAULT NULL,
  `Certificat delictes` tinyint(1) DEFAULT NULL,
  `Titulació` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `entrenadors`
--

INSERT INTO `entrenadors` (`id`, `Nom`, `Llicència`, `Data Naixement`, `Dni`, `Tfn`, `Adreça`, `Població`, `Email`, `Certificat delictes`, `Titulació`) VALUES
(1, 'Adriana Bennasar Morales', 'Entrenador', '31/05/1995', '43198810A', 657311665, 'weyler, 50 ', 'pla de na tesa', 'adrianabennasar1@hotmail.com', NULL, NULL),
(2, 'Beatriz Balmiza Jimenez', 'Entrenador', '02/06/1979', '43106330Y', 646426882, 'Pza Santa Ponsa 3, 2B', 'Calvia', 'beabalmiza@hotmail.com', NULL, '2 NIVELL'),
(3, 'Andreu Massot Isern', 'Entrenador', '24/08/2000', '43478164E', 635642982, 'marti rubi, 99', 'sa cabaneta', NULL, NULL, 'INICIACIÓ'),
(4, 'Rafel Salva Moner', 'Entrenador', '06/04/1995', '43219695G', 654073695, 'C/Ramon y Cajal 17, 5A', 'Palma', 'rafelsalva@hotmail.com', NULL, 'INICIACIÓ'),
(5, 'Antoni Rosselló Mesquida', 'Entrenador', '15/12/1995', '43207745Z', 664272565, 'comuna, 29 ', 'sa cabaneta', 'tonirossellomes@hotmail.com', NULL, 'INICIACIÓ'),
(6, 'David Castilla Morro', 'Entrenador', '07/05/1998', '43460609Q', 663775074, 'Josep verd metge, 1-B; 1ªA', 'portol', NULL, NULL, 'INICIACIÓ'),
(7, 'Guillem Massot Juan', 'Entrenador', '26/09/1971', '34065220n', 638883077, 'martí rubí, 99', 'sa cabaneta', 'massotguillem@gmail.com', NULL, '2 NIVELL'),
(8, 'José Pérez Sureda', 'Entrenador', '13/08/1971', '43067183M', 615518038, 'Inspector Joan Capó 18/A', 'Marratxí', 'jperez@planeta.es', NULL, '1 NIVELL'),
(9, 'Antonio Pou Suarez', 'Entrenador', '02/04/1963', '43006158E', 666335074, 'C/Batre 36', 'Palma', 'tonijoanpou@gmail.com', NULL, '2 NIVELL'),
(10, 'Aina Oliver Pons', 'Entrenador', '22/04/1995', '43209633Q', 616145349, 'son cunill, 25b', 'Pont d\'Inca', 'ainaoliver95@gmail.com', NULL, 'INICIACIÓ'),
(11, 'Marc Isern Landa', 'Entrenador', '03/02/1999', '43465493R', 644254868, 'sant antoni, 17', 'sa cabaneta', NULL, NULL, 'INICIACIÓ'),
(12, 'Jesús González Medina', 'Entrenador', '13/04/1998', '43234557P', 616546688, 'sant pere 17a', 'Portol', 'jesus.gonzalezmedina@gmail.com', NULL, 'INICIACIÓ'),
(13, 'Francisca Colom Amengual', 'Assistent', '19/04/1995', '43467316F', 971602234, 'C/Rectoria 5', 'portol', 'xisca95@terra.es', NULL, NULL),
(14, 'Paula Amengual Belardo', 'Assistent', '08/06/2000', '45691046M', 691597261, 'sant antoni, 10', 'sa cabaneta', NULL, NULL, 'INICIACIÓ'),
(15, 'Esther Amengual Belardo', 'entrenador', '6/5/02', '45691047Y', 691597229, 'sant antoni, 10', 'sa cabaneta', NULL, NULL, 'INICIACIÓ'),
(16, 'Marina Castilla Morro', 'Entrenador', '27/03/02', '45611870H', 657896991, 'Josep verd metge, 1-B; 1ªA', 'portol', NULL, NULL, 'INICIACIÓ'),
(17, 'Maria Magdalena Tugores Montell', 'Entrenador', NULL, NULL, 627525971, NULL, NULL, NULL, NULL, '1 NIVELL'),
(18, 'Pedro Caldentey', 'Entrenador', NULL, NULL, 639221404, '', NULL, NULL, NULL, 'SUPERIOR'),
(19, 'Joan Martorell', 'Entrenador', NULL, NULL, 670373521, NULL, NULL, NULL, NULL, '2 NIVELL'),
(20, 'Miquel Ramis Canyelles', 'Assistent', '21/11/2002', '45698237C', 647939503, 'camí de jardí den ferrer, 7', 'portol', NULL, NULL, 'INICIACIÓ'),
(21, 'Maria del Mar Massot Isern', 'Assistent', '1/8/02', '43478163K', 674089375, 'marti rubi, 99', 'sa cabaneta', NULL, NULL, 'INICIACIÓ'),
(22, 'Javier Alberola Cañellas', 'Assistent', '24/08/01', '45371972X', 722549317, 'escuela nacional, 28', 'son ferriol', NULL, NULL, 'INICIACIÓ'),
(23, 'Anne Jaunarena Villatoro', 'Assistent', '07/07/2000', '45372302H', 634596714, 'sant jaume, 15B', 'portol', NULL, NULL, 'INICIACIÓ'),
(24, 'Juan Andres Vadell Calvo', 'Assistent', '03/05/1997', '43235958Y', 627577782, 'C/Jaume I nº5', 'Santa Maria', 'zxwazzaxz@gmail.com', NULL, NULL),
(25, 'Neus Amengual Cloquell', 'Entrenador', '02/05/2000', '43227779S', 655927853, 'sa torre, 30', 'portol', NULL, NULL, 'INICIACIÓ'),
(26, 'Adjla Kabadi', 'Entrenador', '01/01/2000', 'Y1781519E', 637488653, 'martí rubí, 99', 'sa cabaneta', 'kabadiadjla@gmail.com', NULL, 'INICIACIÓ'),
(27, 'Jose Antonio Domenech Suau', 'delegat camp', '07/09/1972', '43075261X', 971797227, 'Andreu Joan Batle', 'sa cabaneta', 'central@grupodomenech.net', NULL, NULL),
(28, 'Silvia Plaza Garcia', 'delegat camp', '29/04/1976', '43101244A', 617046371, 'Son Ferragut 21', 'marratxi', 'sivipgfj@gmail.com', NULL, NULL),
(29, 'Pep Amengual Rigo', 'delegat camp', '16/04/1971', '43066008A', 654462398, 'sant antoni, 10', 'sa cabaneta', 'pep.amengual@gmail.com', NULL, NULL),
(30, 'Antoni Isern Amengual', 'delegat camp', '14-2-66', '43032859C', 647703108, 'sant antoni, 17', 'sa cabaneta', 'antonisern@gmail.com', NULL, NULL),
(31, 'Jordi Rosselló Mesquida', 'Assistent', '20/09/1999', '45190099K', 648505903, 'comuna 29', 'sa cabaneta', 'jrdirossellomes@gmail.com', NULL, NULL),
(32, 'Juan Antonio Estarellas Mateu', 'Assistent', '18/03/1976', '43102685H', 699421784, 'santa barbara 6', 'sa cabaneta', 'villaciervitos@gmail.com', NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `equip`
--

CREATE TABLE `equip` (
  `id` int(5) UNSIGNED NOT NULL,
  `idcategoria` int(10) UNSIGNED DEFAULT NULL,
  `idtemporada` int(3) UNSIGNED DEFAULT NULL,
  `idgenere` int(1) UNSIGNED DEFAULT NULL,
  `Lletra` char(1) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `equip`
--

INSERT INTO `equip` (`id`, `idcategoria`, `idtemporada`, `idgenere`, `Lletra`) VALUES
(1, 1, 1, NULL, NULL),
(2, 2, 1, NULL, 'B'),
(3, 2, 1, NULL, 'A'),
(4, 3, 1, 2, NULL),
(5, 3, 1, 1, NULL),
(6, 4, 1, 2, 'B'),
(7, 4, 1, 2, 'A'),
(8, 4, 1, 1, NULL),
(9, 5, 1, 2, 'B'),
(10, 5, 1, 2, 'A'),
(11, 5, 1, 1, NULL),
(12, 6, 1, 2, NULL),
(13, 6, 1, 1, NULL),
(14, 7, 1, 2, NULL),
(15, 7, 1, 1, NULL),
(16, 8, 1, 2, NULL),
(17, 8, 1, 1, NULL),
(19, 11, 1, NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `genere`
--

CREATE TABLE `genere` (
  `id` int(1) UNSIGNED NOT NULL,
  `Nom` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `genere`
--

INSERT INTO `genere` (`id`, `Nom`) VALUES
(1, 'Femení'),
(2, 'Masculí'),
(3, 'Mixte');

-- --------------------------------------------------------

--
-- Table structure for table `jugadors`
--

CREATE TABLE `jugadors` (
  `id` int(5) UNSIGNED NOT NULL,
  `dorsal` varchar(255) DEFAULT NULL,
  `nom` varchar(255) DEFAULT NULL,
  `soci` varchar(20) DEFAULT NULL,
  `naixement` date DEFAULT NULL,
  `dni` varchar(255) DEFAULT NULL,
  `tfn1` varchar(255) DEFAULT NULL,
  `tfn2` varchar(255) DEFAULT NULL,
  `adresa` varchar(255) DEFAULT NULL,
  `poblacio` varchar(255) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `idequip` int(5) UNSIGNED DEFAULT NULL,
  `idgen` int(1) UNSIGNED DEFAULT NULL,
  `idclub` int(8) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `jugadors`
--

INSERT INTO `jugadors` (`id`, `dorsal`, `nom`, `soci`, `naixement`, `dni`, `tfn1`, `tfn2`, `adresa`, `poblacio`, `email`, `idequip`, `idgen`, `idclub`) VALUES
(237, '45', 'PROVA', 'NO SOCI', '1999-12-20', '2131231Z', '312312', '312321', 'Olesa 4', 'Sa Cabaneta', 'prueba@prueba.com', 4, NULL, 1),
(238, '45', 'PEP', 'NO SOCI', NULL, '2131231Z', '312312', NULL, NULL, NULL, NULL, NULL, NULL, 1),
(239, '45', 'PEP', 'NO SOCI', NULL, '2131231Z', '312312', NULL, NULL, NULL, NULL, NULL, NULL, 1),
(240, '4', 'Jordi Rosselló', NULL, '1999-12-20', NULL, '666999333', NULL, 'Calle', 'Palma', NULL, NULL, NULL, 1);

-- --------------------------------------------------------

--
-- Table structure for table `pagaments`
--

CREATE TABLE `pagaments` (
  `id` int(6) UNSIGNED NOT NULL,
  `Concepte` varchar(255) NOT NULL,
  `Import` decimal(6,2) NOT NULL,
  `Data` datetime(6) NOT NULL,
  `idjugador` int(5) UNSIGNED DEFAULT NULL,
  `estat` int(1) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `pagaments`
--

INSERT INTO `pagaments` (`id`, `Concepte`, `Import`, `Data`, `idjugador`, `estat`) VALUES
(745, 'Quota', '5.00', '2019-11-27 00:00:00.000000', 237, 2);

-- --------------------------------------------------------

--
-- Table structure for table `participants`
--

CREATE TABLE `participants` (
  `idjug` int(5) UNSIGNED NOT NULL,
  `idact` int(5) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- --------------------------------------------------------

--
-- Table structure for table `preinscripcio`
--

CREATE TABLE `preinscripcio` (
  `id` int(6) UNSIGNED NOT NULL,
  `idjug` int(5) UNSIGNED DEFAULT NULL,
  `idequip` int(5) UNSIGNED DEFAULT NULL,
  `idpagament` int(6) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- --------------------------------------------------------

--
-- Table structure for table `prenda_comanda`
--

CREATE TABLE `prenda_comanda` (
  `idprenda` int(3) UNSIGNED NOT NULL,
  `idcomanda` int(10) UNSIGNED NOT NULL,
  `Talla` varchar(5) NOT NULL,
  `Nom` varchar(255) DEFAULT NULL,
  `Dorsal` int(2) UNSIGNED DEFAULT NULL,
  `idpagament` int(6) UNSIGNED DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- --------------------------------------------------------

--
-- Table structure for table `prendes`
--

CREATE TABLE `prendes` (
  `id` int(3) UNSIGNED NOT NULL,
  `preu_soci` decimal(4,2) NOT NULL,
  `preu_no_soci` decimal(4,2) NOT NULL,
  `Nom` varchar(100) NOT NULL,
  `Talla` tinyint(1) NOT NULL,
  `Num` tinyint(1) NOT NULL,
  `NomP` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `prendes`
--

INSERT INTO `prendes` (`id`, `preu_soci`, `preu_no_soci`, `Nom`, `Talla`, `Num`, `NomP`) VALUES
(1, '20.00', '24.00', 'Dessuadora', 1, 0, 0),
(2, '5.00', '8.00', 'Camiseta tècnica', 1, 0, 1),
(3, '20.00', '24.00', 'Motxilla', 0, 0, 1),
(4, '18.00', '22.00', 'Polo', 1, 0, 0),
(5, '30.00', '35.00', 'Equipació', 1, 1, 1),
(6, '45.00', '50.00', 'Xandall complet', 1, 1, 0),
(7, '30.00', '35.00', 'Jaqueta xandall', 1, 1, 0),
(8, '15.00', '15.00', 'Calçons xandall', 1, 1, 0),
(9, '15.00', '15.00', 'Calçons equipació', 1, 0, 0),
(10, '15.00', '20.00', 'Camiseta equipació', 1, 1, 1);

-- --------------------------------------------------------

--
-- Table structure for table `temporadas`
--

CREATE TABLE `temporadas` (
  `id` int(3) UNSIGNED NOT NULL,
  `Nom` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

--
-- Dumping data for table `temporadas`
--

INSERT INTO `temporadas` (`id`, `Nom`) VALUES
(1, '2019-2020');

-- --------------------------------------------------------

--
-- Table structure for table `usuaris`
--

CREATE TABLE `usuaris` (
  `id` int(10) NOT NULL,
  `usuari` varchar(255) NOT NULL,
  `uuid` varchar(255) NOT NULL,
  `pass` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `usuaris`
--

INSERT INTO `usuaris` (`id`, `usuari`, `uuid`, `pass`) VALUES
(2, 'prova', 'ab5c85f2-3de0-11ea-96d6-ee85ce7c018e', 'hola'),
(3, 'hola', '55910417-3de4-11ea-96d6-ee85ce7c018e', '55T28Bh7PBcMI'),
(4, 'prova', 'eb21bbeb-3de4-11ea-96d6-ee85ce7c018e', 'ebqePeS1KrbRQ'),
(5, 'usuario', 'bfa20bf8-41d0-11ea-96eb-ee86d38f54f7', 'bfaMqq6ejv1R2'),
(6, 'sinclub', '9ae9bf0b-4cc8-11ea-99b1-f6a96a1838d3', '9aiJ3XXERmjTs');

--
-- Triggers `usuaris`
--
DELIMITER $$
CREATE TRIGGER `before_insert_usuaris` BEFORE INSERT ON `usuaris` FOR EACH ROW BEGIN 
SET new.uuid = uuid();
SET new.pass = encrypt(new.pass, new.uuid);
end
$$
DELIMITER ;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `activitats`
--
ALTER TABLE `activitats`
  ADD PRIMARY KEY (`id`) USING BTREE,
  ADD KEY `fk1_activitats` (`idtemporada`) USING BTREE;

--
-- Indexes for table `categoria`
--
ALTER TABLE `categoria`
  ADD PRIMARY KEY (`id`) USING BTREE;

--
-- Indexes for table `clubs`
--
ALTER TABLE `clubs`
  ADD PRIMARY KEY (`id`),
  ADD KEY `fk1_club` (`propietari`);

--
-- Indexes for table `comandes`
--
ALTER TABLE `comandes`
  ADD PRIMARY KEY (`id`) USING BTREE,
  ADD KEY `fk1_comandes` (`idjugador`) USING BTREE;

--
-- Indexes for table `cos tecnic`
--
ALTER TABLE `cos tecnic`
  ADD PRIMARY KEY (`idequip`,`identrenador`) USING BTREE,
  ADD KEY `fk2_cos_tecnic` (`identrenador`) USING BTREE;

--
-- Indexes for table `entrenadors`
--
ALTER TABLE `entrenadors`
  ADD PRIMARY KEY (`id`) USING BTREE;

--
-- Indexes for table `equip`
--
ALTER TABLE `equip`
  ADD PRIMARY KEY (`id`) USING BTREE,
  ADD KEY `fk1_equip` (`idcategoria`) USING BTREE,
  ADD KEY `fk2_equip` (`idtemporada`) USING BTREE,
  ADD KEY `fk3_equip` (`idgenere`) USING BTREE;

--
-- Indexes for table `genere`
--
ALTER TABLE `genere`
  ADD PRIMARY KEY (`id`) USING BTREE;

--
-- Indexes for table `jugadors`
--
ALTER TABLE `jugadors`
  ADD PRIMARY KEY (`id`) USING BTREE,
  ADD KEY `fk1_jugador` (`idequip`) USING BTREE,
  ADD KEY `fk2_jugador` (`idgen`) USING BTREE,
  ADD KEY `fk3_jugador` (`idclub`);

--
-- Indexes for table `pagaments`
--
ALTER TABLE `pagaments`
  ADD PRIMARY KEY (`id`) USING BTREE,
  ADD KEY `fk1_pagament` (`idjugador`) USING BTREE;

--
-- Indexes for table `participants`
--
ALTER TABLE `participants`
  ADD PRIMARY KEY (`idjug`,`idact`) USING BTREE,
  ADD KEY `fk1_participant` (`idact`) USING BTREE,
  ADD KEY `idjug` (`idjug`) USING BTREE;

--
-- Indexes for table `preinscripcio`
--
ALTER TABLE `preinscripcio`
  ADD PRIMARY KEY (`id`) USING BTREE,
  ADD KEY `fk1_preinscripcio` (`idjug`) USING BTREE,
  ADD KEY `fk2_preinscripcio` (`idequip`) USING BTREE,
  ADD KEY `fk3_preinscripcio` (`idpagament`) USING BTREE;

--
-- Indexes for table `prenda_comanda`
--
ALTER TABLE `prenda_comanda`
  ADD PRIMARY KEY (`idprenda`,`idcomanda`) USING BTREE,
  ADD KEY `fk2_prenda_com` (`idcomanda`) USING BTREE,
  ADD KEY `fk3_prenda_com` (`idpagament`) USING BTREE;

--
-- Indexes for table `prendes`
--
ALTER TABLE `prendes`
  ADD PRIMARY KEY (`id`) USING BTREE;

--
-- Indexes for table `temporadas`
--
ALTER TABLE `temporadas`
  ADD PRIMARY KEY (`id`) USING BTREE;

--
-- Indexes for table `usuaris`
--
ALTER TABLE `usuaris`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `activitats`
--
ALTER TABLE `activitats`
  MODIFY `id` int(5) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `categoria`
--
ALTER TABLE `categoria`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;

--
-- AUTO_INCREMENT for table `clubs`
--
ALTER TABLE `clubs`
  MODIFY `id` int(8) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT for table `comandes`
--
ALTER TABLE `comandes`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `entrenadors`
--
ALTER TABLE `entrenadors`
  MODIFY `id` int(3) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;

--
-- AUTO_INCREMENT for table `equip`
--
ALTER TABLE `equip`
  MODIFY `id` int(5) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;

--
-- AUTO_INCREMENT for table `genere`
--
ALTER TABLE `genere`
  MODIFY `id` int(1) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT for table `jugadors`
--
ALTER TABLE `jugadors`
  MODIFY `id` int(5) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=241;

--
-- AUTO_INCREMENT for table `pagaments`
--
ALTER TABLE `pagaments`
  MODIFY `id` int(6) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=746;

--
-- AUTO_INCREMENT for table `preinscripcio`
--
ALTER TABLE `preinscripcio`
  MODIFY `id` int(6) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `prendes`
--
ALTER TABLE `prendes`
  MODIFY `id` int(3) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `temporadas`
--
ALTER TABLE `temporadas`
  MODIFY `id` int(3) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `usuaris`
--
ALTER TABLE `usuaris`
  MODIFY `id` int(10) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `activitats`
--
ALTER TABLE `activitats`
  ADD CONSTRAINT `fk1_activitats` FOREIGN KEY (`idtemporada`) REFERENCES `temporadas` (`id`) ON UPDATE CASCADE;

--
-- Constraints for table `clubs`
--
ALTER TABLE `clubs`
  ADD CONSTRAINT `fk1_club` FOREIGN KEY (`propietari`) REFERENCES `usuaris` (`id`);

--
-- Constraints for table `comandes`
--
ALTER TABLE `comandes`
  ADD CONSTRAINT `fk1_comandes` FOREIGN KEY (`idjugador`) REFERENCES `jugadors` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `cos tecnic`
--
ALTER TABLE `cos tecnic`
  ADD CONSTRAINT `fk1_cos_tecnic` FOREIGN KEY (`idequip`) REFERENCES `equip` (`id`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk2_cos_tecnic` FOREIGN KEY (`identrenador`) REFERENCES `entrenadors` (`id`) ON UPDATE CASCADE;

--
-- Constraints for table `equip`
--
ALTER TABLE `equip`
  ADD CONSTRAINT `fk1_equip` FOREIGN KEY (`idcategoria`) REFERENCES `categoria` (`id`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk2_equip` FOREIGN KEY (`idtemporada`) REFERENCES `temporadas` (`id`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk3_equip` FOREIGN KEY (`idgenere`) REFERENCES `genere` (`id`) ON UPDATE CASCADE;

--
-- Constraints for table `jugadors`
--
ALTER TABLE `jugadors`
  ADD CONSTRAINT `fk1_jugador` FOREIGN KEY (`idequip`) REFERENCES `equip` (`id`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk2_jugador` FOREIGN KEY (`idgen`) REFERENCES `genere` (`id`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk3_jugador` FOREIGN KEY (`idclub`) REFERENCES `clubs` (`id`) ON UPDATE CASCADE;

--
-- Constraints for table `pagaments`
--
ALTER TABLE `pagaments`
  ADD CONSTRAINT `fk1_pagament` FOREIGN KEY (`idjugador`) REFERENCES `jugadors` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `participants`
--
ALTER TABLE `participants`
  ADD CONSTRAINT `fk1_participant` FOREIGN KEY (`idact`) REFERENCES `activitats` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `fk2_participant` FOREIGN KEY (`idjug`) REFERENCES `jugadors` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `preinscripcio`
--
ALTER TABLE `preinscripcio`
  ADD CONSTRAINT `fk1_preinscripcio` FOREIGN KEY (`idjug`) REFERENCES `jugadors` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `fk2_preinscripcio` FOREIGN KEY (`idequip`) REFERENCES `equip` (`id`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk3_preinscripcio` FOREIGN KEY (`idpagament`) REFERENCES `pagaments` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `prenda_comanda`
--
ALTER TABLE `prenda_comanda`
  ADD CONSTRAINT `fk1_prenda_com` FOREIGN KEY (`idprenda`) REFERENCES `prendes` (`id`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk2_prenda_com` FOREIGN KEY (`idcomanda`) REFERENCES `comandes` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `fk3_prenda_com` FOREIGN KEY (`idpagament`) REFERENCES `pagaments` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
