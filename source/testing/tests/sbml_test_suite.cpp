#include "unit_test/UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrTestSuiteModelSimulation.h"
using namespace UnitTest;
using namespace rr;
using namespace std;

extern string             gSBMLModelsPath;
extern string             gCompiler;
extern string             gSupportCodeFolder;
extern string             gTempFolder;
extern string             gTSModelsPath;
//extern string             gDataOutputFolder;

bool RunTest(int number); //Runs both the first and the last version of the test.
bool RunTest(const string& version, int number);
bool CheckLoad(const string& version, int number);
void LoadAndSimulate(const string& version, int caseNumber, RoadRunner& rr, string& dataOutputFolder, TestSuiteModelSimulation& simulation);

SUITE(SBML_test_suite_part1)
{
    //TEST(0) { CHECK(RunTest(1630)); } //Use when need to check one thing.
    TEST(1) { CHECK(RunTest(1)); }
    TEST(2) { CHECK(RunTest(2)); }
    TEST(3) { CHECK(RunTest(3)); }
    TEST(4) { CHECK(RunTest(4)); }
    TEST(5) { CHECK(RunTest(5)); }
    TEST(6) { CHECK(RunTest(6)); }
    TEST(7) { CHECK(RunTest(7)); }
    TEST(8) { CHECK(RunTest(8)); }
    TEST(9) { CHECK(RunTest(9)); }
    TEST(10) { CHECK(RunTest(10)); }
    TEST(11) { CHECK(RunTest(11)); }
    TEST(12) { CHECK(RunTest(12)); }
    TEST(13) { CHECK(RunTest(13)); }
    TEST(14) { CHECK(RunTest(14)); }
    TEST(15) { CHECK(RunTest(15)); }
    TEST(16) { CHECK(RunTest(16)); }
    TEST(17) { CHECK(RunTest(17)); }
    TEST(18) { CHECK(RunTest(18)); }
    TEST(19) { CHECK(RunTest(19)); }
    TEST(20) { CHECK(RunTest(20)); }
    TEST(21) { CHECK(RunTest(21)); }
    TEST(22) { CHECK(RunTest(22)); }
    TEST(23) { CHECK(RunTest(23)); }
    TEST(24) { CHECK(RunTest(24)); }
    TEST(25) { CHECK(RunTest(25)); }
    TEST(26) { CHECK(RunTest(26)); }
    TEST(27) { CHECK(RunTest(27)); }
    TEST(28) { CHECK(RunTest(28)); }
    TEST(29) { CHECK(RunTest(29)); }
    TEST(30) { CHECK(RunTest(30)); }
    TEST(31) { CHECK(RunTest(31)); }
    TEST(32) { CHECK(RunTest(32)); }
    TEST(33) { CHECK(RunTest(33)); }
    TEST(34) { CHECK(RunTest(34)); }
    TEST(35) { CHECK(RunTest(35)); }
    TEST(36) { CHECK(RunTest(36)); }
    TEST(37) { CHECK(RunTest(37)); }
    TEST(38) { CHECK(RunTest(38)); }
    TEST(39) { CHECK(RunTest(39)); }
    TEST(40) { CHECK(RunTest(40)); }
    TEST(41) { CHECK(RunTest(41)); }
    TEST(42) { CHECK(RunTest(42)); }
    TEST(43) { CHECK(RunTest(43)); }
    TEST(44) { CHECK(RunTest(44)); }
    TEST(45) { CHECK(RunTest(45)); }
    TEST(46) { CHECK(RunTest(46)); }
    TEST(47) { CHECK(RunTest(47)); }
    TEST(48) { CHECK(RunTest(48)); }
    TEST(49) { CHECK(RunTest(49)); }
    TEST(50) { CHECK(RunTest(50)); }
    TEST(51) { CHECK(RunTest(51)); }
    TEST(52) { CHECK(RunTest(52)); }
    TEST(53) { CHECK(RunTest(53)); }
    TEST(54) { CHECK(RunTest(54)); }
    TEST(55) { CHECK(RunTest(55)); }
    TEST(56) { CHECK(RunTest(56)); }
    TEST(57) { CHECK(RunTest(57)); }
    TEST(58) { CHECK(RunTest(58)); }
    TEST(59) { CHECK(RunTest(59)); }
    TEST(60) { CHECK(RunTest(60)); }
    TEST(61) { CHECK(RunTest(61)); }
    TEST(62) { CHECK(RunTest(62)); }
    TEST(63) { CHECK(RunTest(63)); }
    TEST(64) { CHECK(RunTest(64)); }
    TEST(65) { CHECK(RunTest(65)); }
    TEST(66) { CHECK(RunTest(66)); }
    TEST(67) { CHECK(RunTest(67)); }
    TEST(68) { CHECK(RunTest(68)); }
    TEST(69) { CHECK(RunTest(69)); }
    TEST(70) { CHECK(RunTest(70)); }
    TEST(71) { CHECK(RunTest(71)); }
    TEST(72) { CHECK(RunTest(72)); }
    TEST(73) { CHECK(RunTest(73)); }
    TEST(74) { CHECK(RunTest(74)); }
    TEST(75) { CHECK(RunTest(75)); }
    TEST(76) { CHECK(RunTest(76)); }
    TEST(77) { CHECK(RunTest(77)); }
    TEST(78) { CHECK(RunTest(78)); }
    TEST(79) { CHECK(RunTest(79)); }
    TEST(80) { CHECK(RunTest(80)); }
    TEST(81) { CHECK(RunTest(81)); }
    TEST(82) { CHECK(RunTest(82)); }
    TEST(83) { CHECK(RunTest(83)); }
    TEST(84) { CHECK(RunTest(84)); }
    TEST(85) { CHECK(RunTest(85)); }
    TEST(86) { CHECK(RunTest(86)); }
    TEST(87) { CHECK(RunTest(87)); }
    TEST(88) { CHECK(RunTest(88)); }
    TEST(89) { CHECK(RunTest(89)); }
    TEST(90) { CHECK(RunTest(90)); }
    TEST(91) { CHECK(RunTest(91)); }
    TEST(92) { CHECK(RunTest(92)); }
    TEST(93) { CHECK(RunTest(93)); }
    TEST(94) { CHECK(RunTest(94)); }
    TEST(95) { CHECK(RunTest(95)); }
    TEST(96) { CHECK(RunTest(96)); }
    TEST(97) { CHECK(RunTest(97)); }
    TEST(98) { CHECK(RunTest(98)); }
    TEST(99) { CHECK(RunTest(99)); }
    TEST(100) { CHECK(RunTest(100)); }
    TEST(101) { CHECK(RunTest(101)); }
    TEST(102) { CHECK(RunTest(102)); }
    TEST(103) { CHECK(RunTest(103)); }
    TEST(104) { CHECK(RunTest(104)); }
    TEST(105) { CHECK(RunTest(105)); }
    TEST(106) { CHECK(RunTest(106)); }
    TEST(107) { CHECK(RunTest(107)); }
    TEST(108) { CHECK(RunTest(108)); }
    TEST(109) { CHECK(RunTest(109)); }
    TEST(110) { CHECK(RunTest(110)); }
    TEST(111) { CHECK(RunTest(111)); }
    TEST(112) { CHECK(RunTest(112)); }
    TEST(113) { CHECK(RunTest(113)); }
    TEST(114) { CHECK(RunTest(114)); }
    TEST(115) { CHECK(RunTest(115)); }
    TEST(116) { CHECK(RunTest(116)); }
    TEST(117) { CHECK(RunTest(117)); }
    TEST(118) { CHECK(RunTest(118)); }
    TEST(119) { CHECK(RunTest(119)); }
    TEST(120) { CHECK(RunTest(120)); }
    TEST(121) { CHECK(RunTest(121)); }
    TEST(122) { CHECK(RunTest(122)); }
    TEST(123) { CHECK(RunTest(123)); }
    TEST(124) { CHECK(RunTest(124)); }
    TEST(125) { CHECK(RunTest(125)); }
    TEST(126) { CHECK(RunTest(126)); }
    TEST(127) { CHECK(RunTest(127)); }
    TEST(128) { CHECK(RunTest(128)); }
    TEST(129) { CHECK(RunTest(129)); }
    TEST(130) { CHECK(RunTest(130)); }
    TEST(131) { CHECK(RunTest(131)); }
    TEST(132) { CHECK(RunTest(132)); }
    TEST(133) { CHECK(RunTest(133)); }
    TEST(134) { CHECK(RunTest(134)); }
    TEST(135) { CHECK(RunTest(135)); }
    TEST(136) { CHECK(RunTest(136)); }
    TEST(137) { CHECK(RunTest(137)); }
    TEST(138) { CHECK(RunTest(138)); }
    TEST(139) { CHECK(RunTest(139)); }
    TEST(140) { CHECK(RunTest(140)); }
    TEST(141) { CHECK(RunTest(141)); }
    TEST(142) { CHECK(RunTest(142)); }
    TEST(143) { CHECK(RunTest(143)); }
    TEST(144) { CHECK(RunTest(144)); }
    TEST(145) { CHECK(RunTest(145)); }
    TEST(146) { CHECK(RunTest(146)); }
    TEST(147) { CHECK(RunTest(147)); }
    TEST(148) { CHECK(RunTest(148)); }
    TEST(149) { CHECK(RunTest(149)); }
    TEST(150) { CHECK(RunTest(150)); }
    TEST(151) { CHECK(RunTest(151)); }
    TEST(152) { CHECK(RunTest(152)); }
    TEST(153) { CHECK(RunTest(153)); }
    TEST(154) { CHECK(RunTest(154)); }
    TEST(155) { CHECK(RunTest(155)); }
    TEST(156) { CHECK(RunTest(156)); }
    TEST(157) { CHECK(RunTest(157)); }
    TEST(158) { CHECK(RunTest(158)); }
    TEST(159) { CHECK(RunTest(159)); }
    TEST(160) { CHECK(RunTest(160)); }
    TEST(161) { CHECK(RunTest(161)); }
    TEST(162) { CHECK(RunTest(162)); }
    TEST(163) { CHECK(RunTest(163)); }
    TEST(164) { CHECK(RunTest(164)); }
    TEST(165) { CHECK(RunTest(165)); }
    TEST(166) { CHECK(RunTest(166)); }
    TEST(167) { CHECK(RunTest(167)); }
    TEST(168) { CHECK(RunTest(168)); }
    TEST(169) { CHECK(RunTest(169)); }
    TEST(170) { CHECK(RunTest(170)); }
    TEST(171) { CHECK(RunTest(171)); }
    TEST(172) { CHECK(RunTest(172)); }
    TEST(173) { CHECK(RunTest(173)); }
    TEST(174) { CHECK(RunTest(174)); }
    TEST(175) { CHECK(RunTest(175)); }
    TEST(176) { CHECK(RunTest(176)); }
    TEST(177) { CHECK(RunTest(177)); }
    TEST(178) { CHECK(RunTest(178)); }
    TEST(179) { CHECK(RunTest(179)); }
    TEST(180) { CHECK(RunTest(180)); }
    TEST(181) { CHECK(RunTest(181)); }
    TEST(182) { CHECK(RunTest(182)); }
    TEST(183) { CHECK(RunTest(183)); }
    TEST(184) { CHECK(RunTest(184)); }
    TEST(185) { CHECK(RunTest(185)); }
    TEST(186) { CHECK(RunTest(186)); }
    TEST(187) { CHECK(RunTest(187)); }
    TEST(188) { CHECK(RunTest(188)); }
    TEST(189) { CHECK(RunTest(189)); }
    TEST(190) { CHECK(RunTest(190)); }
    TEST(191) { CHECK(RunTest(191)); }
    TEST(192) { CHECK(RunTest(192)); }
    TEST(193) { CHECK(RunTest(193)); }
    TEST(194) { CHECK(RunTest(194)); }
    TEST(195) { CHECK(RunTest(195)); }
    TEST(196) { CHECK(RunTest(196)); }
    TEST(197) { CHECK(RunTest(197)); }
    TEST(198) { CHECK(RunTest(198)); }
    TEST(199) { CHECK(RunTest(199)); }
    TEST(200) { CHECK(RunTest(200)); }
    TEST(201) { CHECK(RunTest(201)); }
    TEST(202) { CHECK(RunTest(202)); }
    TEST(203) { CHECK(RunTest(203)); }
    TEST(204) { CHECK(RunTest(204)); }
    TEST(205) { CHECK(RunTest(205)); }
    TEST(206) { CHECK(RunTest(206)); }
    TEST(207) { CHECK(RunTest(207)); }
    TEST(208) { CHECK(RunTest(208)); }
    TEST(209) { CHECK(RunTest(209)); }
    TEST(210) { CHECK(RunTest(210)); }
    TEST(211) { CHECK(RunTest(211)); }
    TEST(212) { CHECK(RunTest(212)); }
    TEST(213) { CHECK(RunTest(213)); }
    TEST(214) { CHECK(RunTest(214)); }
    TEST(215) { CHECK(RunTest(215)); }
    TEST(216) { CHECK(RunTest(216)); }
    TEST(217) { CHECK(RunTest(217)); }
    TEST(218) { CHECK(RunTest(218)); }
    TEST(219) { CHECK(RunTest(219)); }
    TEST(220) { CHECK(RunTest(220)); }
    TEST(221) { CHECK(RunTest(221)); }
    TEST(222) { CHECK(RunTest(222)); }
    TEST(223) { CHECK(RunTest(223)); }
    TEST(224) { CHECK(RunTest(224)); }
    TEST(225) { CHECK(RunTest(225)); }
    TEST(226) { CHECK(RunTest(226)); }
    TEST(227) { CHECK(RunTest(227)); }
    TEST(228) { CHECK(RunTest(228)); }
    TEST(229) { CHECK(RunTest(229)); }
    TEST(230) { CHECK(RunTest(230)); }
    TEST(231) { CHECK(RunTest(231)); }
    TEST(232) { CHECK(RunTest(232)); }
    TEST(233) { CHECK(RunTest(233)); }
    TEST(234) { CHECK(RunTest(234)); }
    TEST(235) { CHECK(RunTest(235)); }
    TEST(236) { CHECK(RunTest(236)); }
    TEST(237) { CHECK(RunTest(237)); }
    TEST(238) { CHECK(RunTest(238)); }
    TEST(239) { CHECK(RunTest(239)); }
    TEST(240) { CHECK(RunTest(240)); }
    TEST(241) { CHECK(RunTest(241)); }
    TEST(242) { CHECK(RunTest(242)); }
    TEST(243) { CHECK(RunTest(243)); }
    TEST(244) { CHECK(RunTest(244)); }
    TEST(245) { CHECK(RunTest(245)); }
    TEST(246) { CHECK(RunTest(246)); }
    TEST(247) { CHECK(RunTest(247)); }
    TEST(248) { CHECK(RunTest(248)); }
    TEST(249) { CHECK(RunTest(249)); }
    TEST(250) { CHECK(RunTest(250)); }
    TEST(251) { CHECK(RunTest(251)); }
    TEST(252) { CHECK(RunTest(252)); }
    TEST(253) { CHECK(RunTest(253)); }
    TEST(254) { CHECK(RunTest(254)); }
    TEST(255) { CHECK(RunTest(255)); }
    TEST(256) { CHECK(RunTest(256)); }
    TEST(257) { CHECK(RunTest(257)); }
    TEST(258) { CHECK(RunTest(258)); }
    TEST(259) { CHECK(RunTest(259)); }
    TEST(260) { CHECK(RunTest(260)); }
    TEST(261) { CHECK(RunTest(261)); }
    TEST(262) { CHECK(RunTest(262)); }
    TEST(263) { CHECK(RunTest(263)); }
    TEST(264) { CHECK(RunTest(264)); }
    TEST(265) { CHECK(RunTest(265)); }
    TEST(266) { CHECK(RunTest(266)); }
    TEST(267) { CHECK(RunTest(267)); }
    TEST(268) { CHECK(RunTest(268)); }
    TEST(269) { CHECK(RunTest(269)); }
    TEST(270) { CHECK(RunTest(270)); }
    TEST(271) { CHECK(RunTest(271)); }
    TEST(272) { CHECK(RunTest(272)); }
    TEST(273) { CHECK(RunTest(273)); }
    TEST(274) { CHECK(RunTest(274)); }
    TEST(275) { CHECK(RunTest(275)); }
    TEST(276) { CHECK(RunTest(276)); }
    TEST(277) { CHECK(RunTest(277)); }
    TEST(278) { CHECK(RunTest(278)); }
    TEST(279) { CHECK(RunTest(279)); }
    TEST(280) { CHECK(RunTest(280)); }
    TEST(281) { CHECK(RunTest(281)); }
    TEST(282) { CHECK(RunTest(282)); }
    TEST(283) { CHECK(RunTest(283)); }
    TEST(284) { CHECK(RunTest(284)); }
    TEST(285) { CHECK(RunTest(285)); }
    TEST(286) { CHECK(RunTest(286)); }
    TEST(287) { CHECK(RunTest(287)); }
    TEST(288) { CHECK(RunTest(288)); }
    TEST(289) { CHECK(RunTest(289)); }
    TEST(290) { CHECK(RunTest(290)); }
    TEST(291) { CHECK(RunTest(291)); }
    TEST(292) { CHECK(RunTest(292)); }
    TEST(293) { CHECK(RunTest(293)); }
    TEST(294) { CHECK(RunTest(294)); }
    TEST(295) { CHECK(RunTest(295)); }
    TEST(296) { CHECK(RunTest(296)); }
    TEST(297) { CHECK(RunTest(297)); }
    TEST(298) { CHECK(RunTest(298)); }
    TEST(299) { CHECK(RunTest(299)); }
    TEST(300) { CHECK(RunTest(300)); }
    TEST(301) { CHECK(RunTest(301)); }
    TEST(302) { CHECK(RunTest(302)); }
    TEST(303) { CHECK(RunTest(303)); }
    TEST(304) { CHECK(RunTest(304)); }
    TEST(305) { CHECK(RunTest(305)); }
    TEST(306) { CHECK(RunTest(306)); }
    TEST(307) { CHECK(RunTest(307)); }
    TEST(308) { CHECK(RunTest(308)); }
    TEST(309) { CHECK(RunTest(309)); }
    TEST(310) { CHECK(RunTest(310)); }
    TEST(311) { CHECK(RunTest(311)); }
    TEST(312) { CHECK(RunTest(312)); }
    TEST(313) { CHECK(RunTest(313)); }
    TEST(314) { CHECK(RunTest(314)); }
    TEST(315) { CHECK(RunTest(315)); }
    TEST(316) { CHECK(RunTest(316)); }
    TEST(317) { CHECK(RunTest(317)); }
    TEST(318) { CHECK(RunTest(318)); }
    TEST(319) { CHECK(RunTest(319)); }
    TEST(320) { CHECK(RunTest(320)); }
    TEST(321) { CHECK(RunTest(321)); }
    TEST(322) { CHECK(RunTest(322)); }
    TEST(323) { CHECK(RunTest(323)); }
    TEST(324) { CHECK(RunTest(324)); }
    TEST(325) { CHECK(RunTest(325)); }
    TEST(326) { CHECK(RunTest(326)); }
    TEST(327) { CHECK(RunTest(327)); }
    TEST(328) { CHECK(RunTest(328)); }
    TEST(329) { CHECK(RunTest(329)); }
    TEST(330) { CHECK(RunTest(330)); }
    TEST(331) { CHECK(RunTest(331)); }
    TEST(332) { CHECK(RunTest(332)); }
    TEST(333) { CHECK(RunTest(333)); }
    TEST(334) { CHECK(RunTest(334)); }
    TEST(335) { CHECK(RunTest(335)); }
    TEST(336) { CHECK(RunTest(336)); }
    TEST(337) { CHECK(RunTest(337)); }
    TEST(338) { CHECK(RunTest(338)); }
    TEST(339) { CHECK(RunTest(339)); }
    TEST(340) { CHECK(RunTest(340)); }
    TEST(341) { CHECK(RunTest(341)); }
    TEST(342) { CHECK(RunTest(342)); }
    TEST(343) { CHECK(RunTest(343)); }
    TEST(344) { CHECK(RunTest(344)); }
    TEST(345) { CHECK(RunTest(345)); }
    TEST(346) { CHECK(RunTest(346)); }
    TEST(347) { CHECK(RunTest(347)); }
    TEST(348) { CHECK(RunTest(348)); }
    TEST(349) { CHECK(RunTest(349)); }
    TEST(350) { CHECK(RunTest(350)); }
    TEST(351) { CHECK(RunTest(351)); }
    TEST(352) { CHECK(RunTest(352)); }
    TEST(353) { CHECK(RunTest(353)); }
    TEST(354) { CHECK(RunTest(354)); }
    TEST(355) { CHECK(RunTest(355)); }
    TEST(356) { CHECK(RunTest(356)); }
    TEST(357) { CHECK(RunTest(357)); }
    TEST(358) { CHECK(RunTest(358)); }
    TEST(359) { CHECK(RunTest(359)); }
    TEST(360) { CHECK(RunTest(360)); }
    TEST(361) { CHECK(RunTest(361)); }
    TEST(362) { CHECK(RunTest(362)); }
    TEST(363) { CHECK(RunTest(363)); }
    TEST(364) { CHECK(RunTest(364)); }
    TEST(365) { CHECK(RunTest(365)); }
    TEST(366) { CHECK(RunTest(366)); }
    TEST(367) { CHECK(RunTest(367)); }
    TEST(368) { CHECK(RunTest(368)); }
    TEST(369) { CHECK(RunTest(369)); }
    TEST(370) { CHECK(RunTest(370)); }
    TEST(371) { CHECK(RunTest(371)); }
    TEST(372) { CHECK(RunTest(372)); }
    TEST(373) { CHECK(RunTest(373)); }
    TEST(374) { CHECK(RunTest(374)); }
    TEST(375) { CHECK(RunTest(375)); }
    TEST(376) { CHECK(RunTest(376)); }
    TEST(377) { CHECK(RunTest(377)); }
    TEST(378) { CHECK(RunTest(378)); }
    TEST(379) { CHECK(RunTest(379)); }
    TEST(380) { CHECK(RunTest(380)); }
    TEST(381) { CHECK(RunTest(381)); }
    TEST(382) { CHECK(RunTest(382)); }
    TEST(383) { CHECK(RunTest(383)); }
    TEST(384) { CHECK(RunTest(384)); }
    TEST(385) { CHECK(RunTest(385)); }
    TEST(386) { CHECK(RunTest(386)); }
    TEST(387) { CHECK(RunTest(387)); }
    TEST(388) { CHECK(RunTest(388)); }
    TEST(389) { CHECK(RunTest(389)); }
    TEST(390) { CHECK(RunTest(390)); }
    TEST(391) { CHECK(RunTest(391)); }
    TEST(392) { CHECK(RunTest(392)); }
    TEST(393) { CHECK(RunTest(393)); }
    TEST(394) { CHECK(RunTest(394)); }
    TEST(395) { CHECK(RunTest(395)); }
    TEST(396) { CHECK(RunTest(396)); }
    TEST(397) { CHECK(RunTest(397)); }
    TEST(398) { CHECK(RunTest(398)); }
    TEST(399) { CHECK(RunTest(399)); }
    TEST(400) { CHECK(RunTest(400)); }
    TEST(401) { CHECK(RunTest(401)); }
    TEST(402) { CHECK(RunTest(402)); }
    TEST(403) { CHECK(RunTest(403)); }
    TEST(404) { CHECK(RunTest(404)); }
    TEST(405) { CHECK(RunTest(405)); }
    TEST(406) { CHECK(RunTest(406)); }
    TEST(407) { CHECK(RunTest(407)); }
    TEST(408) { CHECK(RunTest(408)); }
    TEST(409) { CHECK(RunTest(409)); }
    TEST(410) { CHECK(RunTest(410)); }
    TEST(411) { CHECK(RunTest(411)); }
    TEST(412) { CHECK(RunTest(412)); }
    TEST(413) { CHECK(RunTest(413)); }
    TEST(414) { CHECK(RunTest(414)); }
    TEST(415) { CHECK(RunTest(415)); }
    TEST(416) { CHECK(RunTest(416)); }
    TEST(417) { CHECK(RunTest(417)); }
    TEST(418) { CHECK(RunTest(418)); }
    TEST(419) { CHECK(RunTest(419)); }
    TEST(420) { CHECK(RunTest(420)); }
    TEST(421) { CHECK(RunTest(421)); }
    TEST(422) { CHECK(RunTest(422)); }
    TEST(423) { CHECK(RunTest(423)); }
    TEST(424) { CHECK(RunTest(424)); }
    TEST(425) { CHECK(RunTest(425)); }
    TEST(426) { CHECK(RunTest(426)); }
    TEST(427) { CHECK(RunTest(427)); }
    TEST(428) { CHECK(RunTest(428)); }
    TEST(429) { CHECK(RunTest(429)); }
    TEST(430) { CHECK(RunTest(430)); }
    TEST(431) { CHECK(RunTest(431)); }
    TEST(432) { CHECK(RunTest(432)); }
    TEST(433) { CHECK(RunTest(433)); }
    TEST(434) { CHECK(RunTest(434)); }
    TEST(435) { CHECK(RunTest(435)); }
    TEST(436) { CHECK(RunTest(436)); }
    TEST(437) { CHECK(RunTest(437)); }
    TEST(438) { CHECK(RunTest(438)); }
    TEST(439) { CHECK(RunTest(439)); }
    TEST(440) { CHECK(RunTest(440)); }
    TEST(441) { CHECK(RunTest(441)); }
    TEST(442) { CHECK(RunTest(442)); }
    TEST(443) { CHECK(RunTest(443)); }
    TEST(444) { CHECK(RunTest(444)); }
    TEST(445) { CHECK(RunTest(445)); }
    TEST(446) { CHECK(RunTest(446)); }
    TEST(447) { CHECK(RunTest(447)); }
    TEST(448) { CHECK(RunTest(448)); }
    TEST(449) { CHECK(RunTest(449)); }
    TEST(450) { CHECK(RunTest(450)); }
    TEST(451) { CHECK(RunTest(451)); }
    TEST(452) { CHECK(RunTest(452)); }
    TEST(453) { CHECK(RunTest(453)); }
    TEST(454) { CHECK(RunTest(454)); }
    TEST(455) { CHECK(RunTest(455)); }
    TEST(456) { CHECK(RunTest(456)); }
    TEST(457) { CHECK(RunTest(457)); }
    TEST(458) { CHECK(RunTest(458)); }
    TEST(459) { CHECK(RunTest(459)); }
    TEST(460) { CHECK(RunTest(460)); }
    TEST(461) { CHECK(RunTest(461)); }
    TEST(462) { CHECK(RunTest(462)); }
    TEST(463) { CHECK(RunTest(463)); }
    TEST(464) { CHECK(RunTest(464)); }
    TEST(465) { CHECK(RunTest(465)); }
    TEST(466) { CHECK(RunTest(466)); }
    TEST(467) { CHECK(RunTest(467)); }
    TEST(468) { CHECK(RunTest(468)); }
    TEST(469) { CHECK(RunTest(469)); }
    TEST(470) { CHECK(RunTest(470)); }
    TEST(471) { CHECK(RunTest(471)); }
    TEST(472) { CHECK(RunTest(472)); }
    TEST(473) { CHECK(RunTest(473)); }
    TEST(474) { CHECK(RunTest(474)); }
    TEST(475) { CHECK(RunTest(475)); }
    TEST(476) { CHECK(RunTest(476)); }
    TEST(477) { CHECK(RunTest(477)); }
    TEST(478) { CHECK(RunTest(478)); }
    TEST(479) { CHECK(RunTest(479)); }
    TEST(480) { CHECK(RunTest(480)); }
    TEST(481) { CHECK(RunTest(481)); }
    TEST(482) { CHECK(RunTest(482)); }
    TEST(483) { CHECK(RunTest(483)); }
    TEST(484) { CHECK(RunTest(484)); }
    TEST(485) { CHECK(RunTest(485)); }
    TEST(486) { CHECK(RunTest(486)); }
    TEST(487) { CHECK(RunTest(487)); }
    TEST(488) { CHECK(RunTest(488)); }
    TEST(489) { CHECK(RunTest(489)); }
    TEST(490) { CHECK(RunTest(490)); }
    TEST(491) { CHECK(RunTest(491)); }
    TEST(492) { CHECK(RunTest(492)); }
    TEST(493) { CHECK(RunTest(493)); }
    TEST(494) { CHECK(RunTest(494)); }
    TEST(495) { CHECK(RunTest(495)); }
    TEST(496) { CHECK(RunTest(496)); }
    TEST(497) { CHECK(RunTest(497)); }
    TEST(498) { CHECK(RunTest(498)); }
    TEST(499) { CHECK(RunTest(499)); }
    TEST(500) { CHECK(RunTest(500)); }
    TEST(501) { CHECK(RunTest(501)); }
    TEST(502) { CHECK(RunTest(502)); }
    TEST(503) { CHECK(RunTest(503)); }
    TEST(504) { CHECK(RunTest(504)); }
    TEST(505) { CHECK(RunTest(505)); }
    TEST(506) { CHECK(RunTest(506)); }
    TEST(507) { CHECK(RunTest(507)); }
    TEST(508) { CHECK(RunTest(508)); }
    TEST(509) { CHECK(RunTest(509)); }
    TEST(510) { CHECK(RunTest(510)); }
    TEST(511) { CHECK(RunTest(511)); }
    TEST(512) { CHECK(RunTest(512)); }
    TEST(513) { CHECK(RunTest(513)); }
    TEST(514) { CHECK(RunTest(514)); }
    TEST(515) { CHECK(RunTest(515)); }
    TEST(516) { CHECK(RunTest(516)); }
    TEST(517) { CHECK(RunTest(517)); }
    TEST(518) { CHECK(RunTest(518)); }
    TEST(519) { CHECK(RunTest(519)); }
    TEST(520) { CHECK(RunTest(520)); }
    TEST(521) { CHECK(RunTest(521)); }
    TEST(522) { CHECK(RunTest(522)); }
    TEST(523) { CHECK(RunTest(523)); }
    TEST(524) { CHECK(RunTest(524)); }
    TEST(525) { CHECK(RunTest(525)); }
    TEST(526) { CHECK(RunTest(526)); }
    TEST(527) { CHECK(RunTest(527)); }
    TEST(528) { CHECK(RunTest(528)); }
    TEST(529) { CHECK(RunTest(529)); }
    TEST(530) { CHECK(RunTest(530)); }
    TEST(531) { CHECK(RunTest(531)); }
    TEST(532) { CHECK(RunTest(532)); }
    //TEST(533) { CHECK(RunTest(533)); } // Throws because uninitialized value
    //TEST(534) { CHECK(RunTest(534)); } // Throws because uninitialized value
    TEST(535) { CHECK(RunTest(535)); }
    //TEST(536) { CHECK(RunTest(536)); } // Throws because uninitialized value
    //TEST(537) { CHECK(RunTest(537)); } // Throws because uninitialized value
    //TEST(538) { CHECK(RunTest(538)); } // Throws because uninitialized value
    TEST(539) { CHECK(RunTest(539)); }
    TEST(540) { CHECK(RunTest(540)); }
    TEST(541) { CHECK(RunTest(541)); }
    TEST(542) { CHECK(RunTest(542)); }
    TEST(543) { CHECK(RunTest(543)); }
    TEST(544) { CHECK(RunTest(544)); }
    TEST(545) { CHECK(RunTest(545)); }
    TEST(546) { CHECK(RunTest(546)); }
    TEST(547) { CHECK(RunTest(547)); }
    TEST(548) { CHECK(RunTest(548)); }
    TEST(549) { CHECK(RunTest(549)); }
    TEST(550) { CHECK(RunTest(550)); }
    TEST(551) { CHECK(RunTest(551)); }
    TEST(552) { CHECK(RunTest(552)); }
    TEST(553) { CHECK(RunTest(553)); }
    TEST(554) { CHECK(RunTest(554)); }
    TEST(555) { CHECK(RunTest(555)); }
    TEST(556) { CHECK(RunTest(556)); }
    TEST(557) { CHECK(RunTest(557)); }
    TEST(558) { CHECK(RunTest(558)); }
    TEST(559) { CHECK(RunTest(559)); }
    TEST(560) { CHECK(RunTest(560)); }
    TEST(561) { CHECK(RunTest(561)); }
    TEST(562) { CHECK(RunTest(562)); }
    TEST(563) { CHECK(RunTest(563)); }
    TEST(564) { CHECK(RunTest(564)); }
    TEST(565) { CHECK(RunTest(565)); }
    TEST(566) { CHECK(RunTest(566)); }
    TEST(567) { CHECK(RunTest(567)); }
    TEST(568) { CHECK(RunTest(568)); }
    //TEST(569) { CHECK(RunTest(569)); } // Throws because uninitialized value
    //TEST(570) { CHECK(RunTest(570)); } // Throws because uninitialized value
    TEST(571) { CHECK(RunTest(571)); }
    TEST(572) { CHECK(RunTest(572)); }
    TEST(573) { CHECK(RunTest(573)); }
    TEST(574) { CHECK(RunTest(574)); }
    //TEST(575) { CHECK(RunTest(575)); } // Throws because uninitialized value
    TEST(576) { CHECK(RunTest(576)); }
    TEST(577) { CHECK(RunTest(577)); }
    TEST(578) { CHECK(RunTest(578)); }
    TEST(579) { CHECK(RunTest(579)); }
    TEST(580) { CHECK(RunTest(580)); }
    TEST(581) { CHECK(RunTest(581)); }
    TEST(582) { CHECK(RunTest(582)); }
    TEST(583) { CHECK(RunTest(583)); }
    TEST(584) { CHECK(RunTest(584)); }
    TEST(585) { CHECK(RunTest(585)); }
    TEST(586) { CHECK(RunTest(586)); }
    TEST(587) { CHECK(RunTest(587)); }
    TEST(588) { CHECK(RunTest(588)); }
    TEST(589) { CHECK(RunTest(589)); }
    TEST(590) { CHECK(RunTest(590)); }
    TEST(591) { CHECK(RunTest(591)); }
    TEST(592) { CHECK(RunTest(592)); }
    TEST(593) { CHECK(RunTest(593)); }
    TEST(594) { CHECK(RunTest(594)); }
    TEST(595) { CHECK(RunTest(595)); }
    TEST(596) { CHECK(RunTest(596)); }
    TEST(597) { CHECK(RunTest(597)); }
    TEST(598) { CHECK(RunTest(598)); }
    TEST(599) { CHECK(RunTest(599)); }
    TEST(600) { CHECK(RunTest(600)); }
    TEST(601) { CHECK(RunTest(601)); }
    TEST(602) { CHECK(RunTest(602)); }
    TEST(603) { CHECK(RunTest(603)); }
    TEST(604) { CHECK(RunTest(604)); }
    TEST(605) { CHECK(RunTest(605)); }
    TEST(606) { CHECK(RunTest(606)); }
    TEST(607) { CHECK(RunTest(607)); }
    TEST(608) { CHECK(RunTest(608)); }
    TEST(609) { CHECK(RunTest(609)); }
    TEST(610) { CHECK(RunTest(610)); }
    TEST(611) { CHECK(RunTest(611)); }
    TEST(612) { CHECK(RunTest(612)); }
    TEST(613) { CHECK(RunTest(613)); }
    TEST(614) { CHECK(RunTest(614)); }
    TEST(615) { CHECK(RunTest(615)); }
    TEST(616) { CHECK(RunTest(616)); }
    TEST(617) { CHECK(RunTest(617)); }
    TEST(618) { CHECK(RunTest(618)); }
    TEST(619) { CHECK(RunTest(619)); }
    TEST(620) { CHECK(RunTest(620)); }
    TEST(621) { CHECK(RunTest(621)); }
    TEST(622) { CHECK(RunTest(622)); }
    TEST(623) { CHECK(RunTest(623)); }
    TEST(624) { CHECK(RunTest(624)); }
    TEST(625) { CHECK(RunTest(625)); }
    TEST(626) { CHECK(RunTest(626)); }
    TEST(627) { CHECK(RunTest(627)); }
    TEST(628) { CHECK(RunTest(628)); }
    TEST(629) { CHECK(RunTest(629)); }
    TEST(630) { CHECK(RunTest(630)); }
    TEST(631) { CHECK(RunTest(631)); }
    TEST(632) { CHECK(RunTest(632)); }
    TEST(633) { CHECK(RunTest(633)); }
    TEST(634) { CHECK(RunTest(634)); }
    TEST(635) { CHECK(RunTest(635)); }
    TEST(636) { CHECK(RunTest(636)); }
    TEST(637) { CHECK(RunTest(637)); }
    TEST(638) { CHECK(RunTest(638)); }
    TEST(639) { CHECK(RunTest(639)); }
    TEST(640) { CHECK(RunTest(640)); }
    TEST(641) { CHECK(RunTest(641)); }
    TEST(642) { CHECK(RunTest(642)); }
    TEST(643) { CHECK(RunTest(643)); }
    TEST(644) { CHECK(RunTest(644)); }
    TEST(645) { CHECK(RunTest(645)); }
    TEST(646) { CHECK(RunTest(646)); }
    TEST(647) { CHECK(RunTest(647)); }
    TEST(648) { CHECK(RunTest(648)); }
    TEST(649) { CHECK(RunTest(649)); }
    TEST(650) { CHECK(RunTest(650)); }
    TEST(651) { CHECK(RunTest(651)); }
    TEST(652) { CHECK(RunTest(652)); }
    TEST(653) { CHECK(RunTest(653)); }
    TEST(654) { CHECK(RunTest(654)); }
    TEST(655) { CHECK(RunTest(655)); }
    TEST(656) { CHECK(RunTest(656)); }
    TEST(657) { CHECK(RunTest(657)); }
    TEST(658) { CHECK(RunTest(658)); }
    TEST(659) { CHECK(RunTest(659)); }
    TEST(660) { CHECK(RunTest(660)); }
    TEST(661) { CHECK(RunTest(661)); }
    TEST(662) { CHECK(RunTest(662)); }
    //TEST(663) { CHECK(RunTest(663)); } // Throws because uninitialized value
    //TEST(664) { CHECK(RunTest(664)); } // Throws because uninitialized value
    TEST(665) { CHECK(RunTest(665)); }
    TEST(666) { CHECK(RunTest(666)); }
    TEST(667) { CHECK(RunTest(667)); }
    TEST(668) { CHECK(RunTest(668)); }
    TEST(669) { CHECK(RunTest(669)); }
    TEST(670) { CHECK(RunTest(670)); }
    TEST(671) { CHECK(RunTest(671)); }
    TEST(672) { CHECK(RunTest(672)); }
    TEST(673) { CHECK(RunTest(673)); }
    TEST(674) { CHECK(RunTest(674)); }
    TEST(675) { CHECK(RunTest(675)); }
    TEST(676) { CHECK(RunTest(676)); }
    TEST(677) { CHECK(RunTest(677)); }
    TEST(678) { CHECK(RunTest(678)); }
    TEST(679) { CHECK(RunTest(679)); }
    TEST(680) { CHECK(RunTest(680)); }
    TEST(681) { CHECK(RunTest(681)); }
    TEST(682) { CHECK(RunTest(682)); }
    TEST(683) { CHECK(RunTest(683)); }
    TEST(684) { CHECK(RunTest(684)); }
    TEST(685) { CHECK(RunTest(685)); }
    TEST(686) { CHECK(RunTest(686)); }
    TEST(687) { CHECK(RunTest(687)); }
    TEST(688) { CHECK(RunTest(688)); }
    TEST(689) { CHECK(RunTest(689)); }
    TEST(690) { CHECK(RunTest(690)); }
    TEST(691) { CHECK(RunTest(691)); }
    TEST(692) { CHECK(RunTest(692)); }
    TEST(693) { CHECK(RunTest(693)); }
    TEST(694) { CHECK(RunTest(694)); }
    TEST(695) { CHECK(RunTest(695)); }
    TEST(696) { CHECK(RunTest(696)); }
    TEST(697) { CHECK(RunTest(697)); }
    TEST(698) { CHECK(RunTest(698)); }
    TEST(699) { CHECK(RunTest(699)); }
    TEST(700) { CHECK(RunTest(700)); }
    TEST(701) { CHECK(RunTest(701)); }
    TEST(702) { CHECK(RunTest(702)); }
    TEST(703) { CHECK(RunTest(703)); }
    TEST(704) { CHECK(RunTest(704)); }
    TEST(705) { CHECK(RunTest(705)); }
    TEST(706) { CHECK(RunTest(706)); }
    TEST(707) { CHECK(RunTest(707)); }
    TEST(708) { CHECK(RunTest(708)); }
    TEST(709) { CHECK(RunTest(709)); }
    TEST(710) { CHECK(RunTest(710)); }
    TEST(711) { CHECK(RunTest(711)); }
    TEST(712) { CHECK(RunTest(712)); }
    TEST(713) { CHECK(RunTest(713)); }
    TEST(714) { CHECK(RunTest(714)); }
    TEST(715) { CHECK(RunTest(715)); }
    TEST(716) { CHECK(RunTest(716)); }
    TEST(717) { CHECK(RunTest(717)); }
    TEST(718) { CHECK(RunTest(718)); }
    TEST(719) { CHECK(RunTest(719)); }
    TEST(720) { CHECK(RunTest(720)); }
    TEST(721) { CHECK(RunTest(721)); }
    TEST(722) { CHECK(RunTest(722)); }
    TEST(723) { CHECK(RunTest(723)); }
    TEST(724) { CHECK(RunTest(724)); }
    TEST(725) { CHECK(RunTest(725)); }
    TEST(726) { CHECK(RunTest(726)); }
    TEST(727) { CHECK(RunTest(727)); }
    TEST(728) { CHECK(RunTest(728)); }
    TEST(729) { CHECK(RunTest(729)); }
    TEST(730) { CHECK(RunTest(730)); }
    TEST(731) { CHECK(RunTest(731)); }
    TEST(732) { CHECK(RunTest(732)); }
    TEST(733) { CHECK(RunTest(733)); }
    TEST(734) { CHECK(RunTest(734)); }
    TEST(735) { CHECK(RunTest(735)); }
    TEST(736) { CHECK(RunTest(736)); }
    TEST(737) { CHECK(RunTest(737)); }
    TEST(738) { CHECK(RunTest(738)); }
    TEST(739) { CHECK(RunTest(739)); }
    TEST(740) { CHECK(RunTest(740)); }
    TEST(741) { CHECK(RunTest(741)); }
    TEST(742) { CHECK(RunTest(742)); }
    TEST(743) { CHECK(RunTest(743)); }
    TEST(744) { CHECK(RunTest(744)); }
    TEST(745) { CHECK(RunTest(745)); }
    TEST(746) { CHECK(RunTest(746)); }
    TEST(747) { CHECK(RunTest(747)); }
    TEST(748) { CHECK(RunTest(748)); }
    TEST(749) { CHECK(RunTest(749)); }
    TEST(750) { CHECK(RunTest(750)); }
    TEST(751) { CHECK(RunTest(751)); }
    TEST(752) { CHECK(RunTest(752)); }
    TEST(753) { CHECK(RunTest(753)); }
    TEST(754) { CHECK(RunTest(754)); }
    TEST(755) { CHECK(RunTest(755)); }
    TEST(756) { CHECK(RunTest(756)); }
    TEST(757) { CHECK(RunTest(757)); }
    TEST(758) { CHECK(RunTest(758)); }
    TEST(759) { CHECK(RunTest(759)); }
    TEST(760) { CHECK(RunTest(760)); }
    TEST(761) { CHECK(RunTest(761)); }
    //TEST(762) { CHECK(RunTest(762)); } // Throws because uninitialized value
    TEST(763) { CHECK(RunTest(763)); }
    TEST(764) { CHECK(RunTest(764)); }
    TEST(765) { CHECK(RunTest(765)); }
    TEST(766) { CHECK(RunTest(766)); }
    TEST(767) { CHECK(RunTest(767)); }
    TEST(768) { CHECK(RunTest(768)); }
    TEST(769) { CHECK(RunTest(769)); }
    TEST(770) { CHECK(RunTest(770)); }
    TEST(771) { CHECK(RunTest(771)); }
    TEST(772) { CHECK(RunTest(772)); }
    TEST(773) { CHECK(RunTest(773)); }
    TEST(774) { CHECK(RunTest(774)); }
    TEST(775) { CHECK(RunTest(775)); }
    TEST(776) { CHECK(RunTest(776)); }
    //TEST(777) { CHECK(RunTest(777)); } // Throws because uninitialized value
    TEST(778) { CHECK(RunTest(778)); }
    TEST(779) { CHECK(RunTest(779)); }
    TEST(780) { CHECK(RunTest(780)); }
    TEST(781) { CHECK(RunTest(781)); }
    TEST(782) { CHECK(RunTest(782)); }
    TEST(783) { CHECK(RunTest(783)); }
    TEST(784) { CHECK(RunTest(784)); }
    TEST(785) { CHECK(RunTest(785)); }
    TEST(786) { CHECK(RunTest(786)); }
    TEST(787) { CHECK(RunTest(787)); }
    TEST(788) { CHECK(RunTest(788)); }
    TEST(789) { CHECK(RunTest(789)); }
    TEST(790) { CHECK(RunTest(790)); }
    TEST(791) { CHECK(RunTest(791)); }
    TEST(792) { CHECK(RunTest(792)); }
    TEST(793) { CHECK(RunTest(793)); }
    TEST(794) { CHECK(RunTest(794)); }
    TEST(795) { CHECK(RunTest(795)); }
    TEST(796) { CHECK(RunTest(796)); }
    TEST(797) { CHECK(RunTest(797)); }
    TEST(798) { CHECK(RunTest(798)); }
    TEST(799) { CHECK(RunTest(799)); }
    TEST(800) { CHECK(RunTest(800)); }
    TEST(801) { CHECK(RunTest(801)); }
    TEST(802) { CHECK(RunTest(802)); }
    TEST(803) { CHECK(RunTest(803)); }
    TEST(804) { CHECK(RunTest(804)); }
    TEST(805) { CHECK(RunTest(805)); }
    TEST(806) { CHECK(RunTest(806)); }
    TEST(807) { CHECK(RunTest(807)); }
    TEST(808) { CHECK(RunTest(808)); }
    TEST(809) { CHECK(RunTest(809)); }
    TEST(810) { CHECK(RunTest(810)); }
    TEST(811) { CHECK(RunTest(811)); }
    TEST(812) { CHECK(RunTest(812)); }
    TEST(813) { CHECK(RunTest(813)); }
    TEST(814) { CHECK(RunTest(814)); }
    TEST(815) { CHECK(RunTest(815)); }
    TEST(816) { CHECK(RunTest(816)); }
    TEST(817) { CHECK(RunTest(817)); }
    TEST(818) { CHECK(RunTest(818)); }
    TEST(819) { CHECK(RunTest(819)); }
    TEST(820) { CHECK(RunTest(820)); }
    TEST(821) { CHECK(RunTest(821)); }
    TEST(822) { CHECK(RunTest(822)); }
    TEST(823) { CHECK(RunTest(823)); }
    TEST(824) { CHECK(RunTest(824)); }
    TEST(825) { CHECK(RunTest(825)); }
    TEST(826) { CHECK(RunTest(826)); }
    TEST(827) { CHECK(RunTest(827)); }
    TEST(828) { CHECK(RunTest(828)); }
    TEST(829) { CHECK(RunTest(829)); }
    TEST(830) { CHECK(RunTest(830)); }
    TEST(831) { CHECK(RunTest(831)); }
    TEST(832) { CHECK(RunTest(832)); }
    TEST(833) { CHECK(RunTest(833)); }
    TEST(834) { CHECK(RunTest(834)); }
    TEST(835) { CHECK(RunTest(835)); }
    TEST(836) { CHECK(RunTest(836)); }
    TEST(837) { CHECK(RunTest(837)); }
    TEST(838) { CHECK(RunTest(838)); }
    TEST(839) { CHECK(RunTest(839)); }
    TEST(840) { CHECK(RunTest(840)); }
    TEST(841) { CHECK(RunTest(841)); }
    TEST(842) { CHECK(RunTest(842)); }
    TEST(843) { CHECK(RunTest(843)); }
    //TEST(844) { CHECK(RunTest(844)); } // Throws because uninitialized value
    TEST(845) { CHECK(RunTest(845)); }
    TEST(846) { CHECK(RunTest(846)); }
    TEST(847) { CHECK(RunTest(847)); }
    TEST(848) { CHECK(RunTest(848)); }
    TEST(849) { CHECK(RunTest(849)); }
    TEST(850) { CHECK(RunTest(850)); }
    TEST(851) { CHECK(RunTest(851)); }
    TEST(852) { CHECK(RunTest(852)); }
    TEST(853) { CHECK(RunTest(853)); }
    TEST(854) { CHECK(RunTest(854)); }
    TEST(855) { CHECK(RunTest(855)); }
    TEST(856) { CHECK(RunTest(856)); }
    TEST(857) { CHECK(RunTest(857)); }
    TEST(858) { CHECK(RunTest(858)); }
    TEST(859) { CHECK(RunTest(859)); }
    TEST(860) { CHECK(RunTest(860)); }
    TEST(861) { CHECK(RunTest(861)); }
    TEST(862) { CHECK(RunTest(862)); }
    TEST(863) { CHECK(RunTest(863)); }
    TEST(864) { CHECK(RunTest(864)); }
    TEST(865) { CHECK(RunTest(865)); }
    TEST(866) { CHECK(RunTest(866)); }
    TEST(867) { CHECK(RunTest(867)); }
    TEST(868) { CHECK(RunTest(868)); }
    TEST(869) { CHECK(RunTest(869)); }
    TEST(870) { CHECK(RunTest(870)); }
    TEST(871) { CHECK(RunTest(871)); }
    TEST(872) { CHECK(RunTest(872)); }
    TEST(873) { CHECK(RunTest(873)); }
    TEST(874) { CHECK(RunTest(874)); }
    TEST(875) { CHECK(RunTest(875)); }
    TEST(876) { CHECK(RunTest(876)); }
    TEST(877) { CHECK(RunTest(877)); }
    TEST(878) { CHECK(RunTest(878)); }
    TEST(879) { CHECK(RunTest(879)); }
    TEST(880) { CHECK(RunTest(880)); }
    TEST(881) { CHECK(RunTest(881)); }
    TEST(882) { CHECK(RunTest(882)); }
    TEST(883) { CHECK(RunTest(883)); }
    TEST(884) { CHECK(RunTest(884)); }
    TEST(885) { CHECK(RunTest(885)); }
    TEST(886) { CHECK(RunTest(886)); }
    TEST(887) { CHECK(RunTest(887)); }
    TEST(888) { CHECK(RunTest(888)); }
    TEST(889) { CHECK(RunTest(889)); }
    TEST(890) { CHECK(RunTest(890)); }
    TEST(891) { CHECK(RunTest(891)); }
    TEST(892) { CHECK(RunTest(892)); }
    TEST(893) { CHECK(RunTest(893)); }
    TEST(894) { CHECK(RunTest(894)); }
    TEST(895) { CHECK(RunTest(895)); }
    TEST(896) { CHECK(RunTest(896)); }
    TEST(897) { CHECK(RunTest(897)); }
    TEST(898) { CHECK(RunTest(898)); }
    TEST(899) { CHECK(RunTest(899)); }
    TEST(900) { CHECK(RunTest(900)); }
    TEST(901) { CHECK(RunTest(901)); }
    TEST(902) { CHECK(RunTest(902)); }
    TEST(903) { CHECK(RunTest(903)); }
    TEST(904) { CHECK(RunTest(904)); }
    TEST(905) { CHECK(RunTest(905)); }
    TEST(906) { CHECK(RunTest(906)); }
    TEST(907) { CHECK(RunTest(907)); }
    TEST(908) { CHECK(RunTest(908)); }
    TEST(909) { CHECK(RunTest(909)); }
    TEST(910) { CHECK(RunTest(910)); }
    TEST(911) { CHECK(RunTest(911)); }
    TEST(912) { CHECK(RunTest(912)); }
    TEST(913) { CHECK(RunTest(913)); }
    TEST(914) { CHECK(RunTest(914)); }
    TEST(915) { CHECK(RunTest(915)); }
    TEST(916) { CHECK(RunTest(916)); }
    TEST(917) { CHECK(RunTest(917)); }
    TEST(918) { CHECK(RunTest(918)); }
    TEST(919) { CHECK(RunTest(919)); }
    TEST(920) { CHECK(RunTest(920)); }
    TEST(921) { CHECK(RunTest(921)); }
    TEST(922) { CHECK(RunTest(922)); }
    TEST(923) { CHECK(RunTest(923)); }
    TEST(924) { CHECK(RunTest(924)); }
    TEST(925) { CHECK(RunTest(925)); }
    TEST(926) { CHECK(RunTest(926)); }
    TEST(927) { CHECK(RunTest(927)); }
    TEST(928) { CHECK(RunTest(928)); }
    TEST(929) { CHECK(RunTest(929)); }
    TEST(930) { CHECK(RunTest(930)); }
    TEST(931) { CHECK(RunTest(931)); }
    TEST(932) { CHECK(RunTest(932)); }
    TEST(933) { CHECK(RunTest(933)); }
    TEST(934) { CHECK(RunTest(934)); }
    TEST(935) { CHECK(RunTest(935)); }
    TEST(936) { CHECK(RunTest(936)); }
    TEST(937) { CHECK(RunTest(937)); }
    TEST(938) { CHECK(RunTest(938)); }
    TEST(939) { CHECK(RunTest(939)); }
    TEST(940) { CHECK(RunTest(940)); }
    TEST(941) { CHECK(RunTest(941)); }
    TEST(942) { CHECK(RunTest(942)); }
    TEST(943) { CHECK(RunTest(943)); }
    TEST(944) { CHECK(RunTest(944)); }
    TEST(945) { CHECK(RunTest(945)); }
    TEST(946) { CHECK(RunTest(946)); }
    TEST(947) { CHECK(RunTest(947)); }
    TEST(948) { CHECK(RunTest(948)); }
    TEST(949) { CHECK(RunTest(949)); }
    TEST(950) { CHECK(RunTest(950)); }
    TEST(951) { CHECK(RunTest(951)); }
    TEST(952) { CHECK(RunTest(952)); }
    TEST(953) { CHECK(RunTest(953)); }
    TEST(954) { CHECK(RunTest(954)); }
    TEST(955) { CHECK(RunTest(955)); }
    TEST(956) { CHECK(RunTest(956)); }
    TEST(957) { CHECK(RunTest(957)); }
    TEST(958) { CHECK(RunTest(958)); }
    TEST(959) { CHECK(RunTest(959)); }
    TEST(960) { CHECK(RunTest(960)); }
    TEST(961) { CHECK(RunTest(961)); }
    TEST(962) { CHECK(RunTest(962)); }
    TEST(963) { CHECK(RunTest(963)); }
    TEST(964) { CHECK(RunTest(964)); }
    TEST(965) { CHECK(RunTest(965)); }
    TEST(966) { CHECK(RunTest(966)); }
    TEST(967) { CHECK(RunTest(967)); }
    TEST(968) { CHECK(RunTest(968)); }
    TEST(969) { CHECK(RunTest(969)); }
    TEST(970) { CHECK(RunTest(970)); }
    TEST(971) { CHECK(RunTest(971)); }
    TEST(972) { CHECK(RunTest(972)); }
    //TEST(973) { CHECK(RunTest(973)); } //l2v1 fails, but l2v5 passes.
    TEST(974) { CHECK(RunTest(974)); }
    TEST(975) { CHECK(RunTest(975)); }
    TEST(976) { CHECK(RunTest(976)); }
    TEST(977) { CHECK(RunTest(977)); }
    TEST(978) { CHECK(RunTest(978)); }
    TEST(979) { CHECK(RunTest(979)); }
    TEST(980) { CHECK(RunTest(980)); }
    TEST(981) { CHECK(RunTest(981)); }
    TEST(982) { CHECK(RunTest(982)); }
    //TEST(983) { CHECK(RunTest(983)); } // Throws because uninitialized value
    TEST(984) { CHECK(RunTest(984)); }
    TEST(985) { CHECK(RunTest(985)); }
    TEST(986) { CHECK(RunTest(986)); }
    TEST(987) { CHECK(RunTest(987)); }
    TEST(988) { CHECK(RunTest(988)); }
    //TEST(989) { CHECK(RunTest(989)); } //l2v1 fails, l2v5 passes
    //TEST(990) { CHECK(RunTest(990)); } //l2v1 fails, l2v5 passes
    //TEST(991) { CHECK(RunTest(991)); } //l2v1 fails, l2v5 passes
    //TEST(992) { CHECK(RunTest(992)); } //l2v1 fails, l2v5 passes
    //TEST(993) { CHECK(RunTest(993)); } // Throws because uninitialized value
    //TEST(994) { CHECK(RunTest(994)); } //l2v1 fails, l2v5 passes
    TEST(995) { CHECK(RunTest(995)); }
    TEST(996) { CHECK(RunTest(996)); }
    TEST(997) { CHECK(RunTest(997)); }
    TEST(998) { CHECK(RunTest(998)); }
    TEST(999) { CHECK(RunTest(999)); }
}

SUITE(SBML_test_suite_part2)
{
    TEST(1000) { CHECK(RunTest(1000)); }
    TEST(1001) { CHECK(RunTest(1001)); }
    TEST(1002) { CHECK(RunTest(1002)); }
    TEST(1003) { CHECK(RunTest(1003)); }
    TEST(1004) { CHECK(RunTest(1004)); }
    TEST(1005) { CHECK(RunTest(1005)); }
    TEST(1006) { CHECK(RunTest(1006)); }
    TEST(1007) { CHECK(RunTest(1007)); }
    TEST(1008) { CHECK(RunTest(1008)); }
    TEST(1009) { CHECK(RunTest(1009)); }
    TEST(1010) { CHECK(RunTest(1010)); }
    TEST(1011) { CHECK(RunTest(1011)); }
    TEST(1012) { CHECK(RunTest(1012)); }
    TEST(1013) { CHECK(RunTest(1013)); }
    TEST(1014) { CHECK(RunTest(1014)); }
    TEST(1015) { CHECK(RunTest(1015)); }
    TEST(1016) { CHECK(RunTest(1016)); }
    TEST(1017) { CHECK(RunTest(1017)); }
    TEST(1018) { CHECK(RunTest(1018)); }
    TEST(1019) { CHECK(RunTest(1019)); }
    TEST(1020) { CHECK(RunTest(1020)); }
    TEST(1021) { CHECK(RunTest(1021)); }
    TEST(1022) { CHECK(RunTest(1022)); }
    TEST(1023) { CHECK(RunTest(1023)); }
    TEST(1024) { CHECK(RunTest(1024)); }
    TEST(1025) { CHECK(RunTest(1025)); }
    TEST(1026) { CHECK(RunTest(1026)); }
    TEST(1027) { CHECK(RunTest(1027)); }
    TEST(1028) { CHECK(RunTest(1028)); }
    TEST(1029) { CHECK(RunTest(1029)); }
    TEST(1030) { CHECK(RunTest(1030)); }
    TEST(1031) { CHECK(RunTest(1031)); }
    TEST(1032) { CHECK(RunTest(1032)); }
    TEST(1033) { CHECK(RunTest(1033)); }
    TEST(1034) { CHECK(RunTest(1034)); }
    TEST(1035) { CHECK(RunTest(1035)); }
    TEST(1036) { CHECK(RunTest(1036)); }
    TEST(1037) { CHECK(RunTest(1037)); }
    TEST(1038) { CHECK(RunTest(1038)); }
    TEST(1039) { CHECK(RunTest(1039)); }
    TEST(1040) { CHECK(RunTest(1040)); }
    TEST(1041) { CHECK(RunTest(1041)); }
    TEST(1042) { CHECK(RunTest(1042)); }
    TEST(1043) { CHECK(RunTest(1043)); }
    //TEST(1044) { CHECK(RunTest(1044)); } // Throws because uninitialized value
    TEST(1045) { CHECK(RunTest(1045)); }
    TEST(1046) { CHECK(RunTest(1046)); }
    TEST(1047) { CHECK(RunTest(1047)); }
    TEST(1048) { CHECK(RunTest(1048)); }
    TEST(1049) { CHECK(RunTest(1049)); }
    TEST(1050) { CHECK(RunTest(1050)); }
    TEST(1051) { CHECK(RunTest(1051)); }
    TEST(1052) { CHECK(RunTest(1052)); }
    TEST(1053) { CHECK(RunTest(1053)); }
    TEST(1054) { CHECK(RunTest(1054)); }
    TEST(1055) { CHECK(RunTest(1055)); }
    TEST(1056) { CHECK(RunTest(1056)); }
    TEST(1057) { CHECK(RunTest(1057)); }
    TEST(1058) { CHECK(RunTest(1058)); }
    TEST(1059) { CHECK(RunTest(1059)); }
    TEST(1060) { CHECK(RunTest(1060)); }
    TEST(1061) { CHECK(RunTest(1061)); }
    TEST(1062) { CHECK(RunTest(1062)); }
    TEST(1063) { CHECK(RunTest(1063)); }
    TEST(1064) { CHECK(RunTest(1064)); }
    TEST(1065) { CHECK(RunTest(1065)); }
    TEST(1066) { CHECK(RunTest(1066)); }
    TEST(1067) { CHECK(RunTest(1067)); }
    TEST(1068) { CHECK(RunTest(1068)); }
    TEST(1069) { CHECK(RunTest(1069)); }
    TEST(1070) { CHECK(RunTest(1070)); }
    TEST(1071) { CHECK(RunTest(1071)); }
    TEST(1072) { CHECK(RunTest(1072)); }
    TEST(1073) { CHECK(RunTest(1073)); }
    TEST(1074) { CHECK(RunTest(1074)); }
    TEST(1075) { CHECK(RunTest(1075)); }
    TEST(1076) { CHECK(RunTest(1076)); }
    TEST(1077) { CHECK(RunTest(1077)); }
    TEST(1078) { CHECK(RunTest(1078)); }
    TEST(1079) { CHECK(RunTest(1079)); }
    TEST(1080) { CHECK(RunTest(1080)); }
    TEST(1081) { CHECK(RunTest(1081)); }
    TEST(1082) { CHECK(RunTest(1082)); }
    TEST(1083) { CHECK(RunTest(1083)); }
    TEST(1084) { CHECK(RunTest(1084)); }
    TEST(1085) { CHECK(RunTest(1085)); }
    TEST(1086) { CHECK(RunTest(1086)); }
    TEST(1087) { CHECK(RunTest(1087)); }
    TEST(1088) { CHECK(RunTest(1088)); }
    TEST(1089) { CHECK(RunTest(1089)); }
    TEST(1090) { CHECK(RunTest(1090)); }
    TEST(1091) { CHECK(RunTest(1091)); }
    TEST(1092) { CHECK(RunTest(1092)); }
    TEST(1093) { CHECK(RunTest(1093)); }
    TEST(1094) { CHECK(RunTest(1094)); }
    TEST(1095) { CHECK(RunTest(1095)); }
    TEST(1096) { CHECK(RunTest(1096)); }
    TEST(1097) { CHECK(RunTest(1097)); }
    TEST(1098) { CHECK(RunTest(1098)); }
    TEST(1099) { CHECK(RunTest(1099)); }
    TEST(1100) { CHECK(RunTest(1100)); }
    TEST(1101) { CHECK(RunTest(1101)); }
    TEST(1102) { CHECK(RunTest(1102)); }
    TEST(1103) { CHECK(RunTest(1103)); }
    TEST(1104) { CHECK(RunTest(1104)); }
    TEST(1105) { CHECK(RunTest(1105)); }
    TEST(1106) { CHECK(RunTest(1106)); }
    TEST(1107) { CHECK(RunTest(1107)); }
    //TEST(1108) { CHECK(RunTest(1108)); } // Throws because uninitialized value
    TEST(1109) { CHECK(RunTest(1109)); }
    TEST(1110) { CHECK(RunTest(1110)); }
    TEST(1111) { CHECK(RunTest(1111)); }
    TEST(1112) { CHECK(RunTest(1112)); }
    TEST(1113) { CHECK(RunTest(1113)); }
    TEST(1114) { CHECK(RunTest(1114)); }
    TEST(1115) { CHECK(RunTest(1115)); }
    TEST(1116) { CHECK(RunTest(1116)); }
    TEST(1117) { CHECK(RunTest(1117)); }
    TEST(1118) { CHECK(RunTest(1118)); }
    TEST(1119) { CHECK(RunTest(1119)); }
    TEST(1120) { CHECK(RunTest(1120)); }
    TEST(1121) { CHECK(RunTest(1121)); }
    TEST(1122) { CHECK(RunTest(1122)); }
    TEST(1123) { CHECK(RunTest(1123)); }
    TEST(1124) { CHECK(RunTest(1124)); }
    TEST(1125) { CHECK(RunTest(1125)); }
    TEST(1126) { CHECK(RunTest(1126)); }
    TEST(1127) { CHECK(RunTest(1127)); }
    TEST(1128) { CHECK(RunTest(1128)); }
    TEST(1129) { CHECK(RunTest(1129)); }
    TEST(1130) { CHECK(RunTest(1130)); }
    TEST(1131) { CHECK(RunTest(1131)); }
    TEST(1132) { CHECK(RunTest(1132)); }
    TEST(1133) { CHECK(RunTest(1133)); }
    TEST(1134) { CHECK(RunTest(1134)); }
    TEST(1135) { CHECK(RunTest(1135)); }
    TEST(1136) { CHECK(RunTest(1136)); }
    TEST(1137) { CHECK(RunTest(1137)); }
    TEST(1138) { CHECK(RunTest(1138)); }
    TEST(1139) { CHECK(RunTest(1139)); }
    TEST(1140) { CHECK(RunTest(1140)); }
    TEST(1141) { CHECK(RunTest(1141)); }
    TEST(1142) { CHECK(RunTest(1142)); }
    TEST(1143) { CHECK(RunTest(1143)); }
    TEST(1144) { CHECK(RunTest(1144)); }
    TEST(1145) { CHECK(RunTest(1145)); }
    TEST(1146) { CHECK(RunTest(1146)); }
    TEST(1147) { CHECK(RunTest(1147)); }
    TEST(1148) { CHECK(RunTest(1148)); }
    TEST(1149) { CHECK(RunTest(1149)); }
    TEST(1150) { CHECK(RunTest(1150)); }
    TEST(1151) { CHECK(RunTest(1151)); }
    TEST(1152) { CHECK(RunTest(1152)); }
    TEST(1153) { CHECK(RunTest(1153)); }
    TEST(1154) { CHECK(RunTest(1154)); }
    TEST(1155) { CHECK(RunTest(1155)); }
    TEST(1156) { CHECK(RunTest(1156)); }
    TEST(1157) { CHECK(RunTest(1157)); }
    TEST(1158) { CHECK(RunTest(1158)); }
    TEST(1159) { CHECK(RunTest(1159)); }
    TEST(1160) { CHECK(RunTest(1160)); }
    TEST(1161) { CHECK(RunTest(1161)); }
    TEST(1162) { CHECK(RunTest(1162)); }
    TEST(1163) { CHECK(RunTest(1163)); }
    TEST(1164) { CHECK(RunTest(1164)); }
    //TEST(1165) { CHECK(RunTest(1165)); } //Fail due to l3v2 flattening
    TEST(1166) { CHECK(RunTest(1166)); }
    //TEST(1167) { CHECK(RunTest(1167)); } //Fail due to l3v2 flattening
    //TEST(1168) { CHECK(RunTest(1168)); } //Fail due to l3v2 flattening
    TEST(1169) { CHECK(RunTest(1169)); }
    TEST(1170) { CHECK(RunTest(1170)); }
    TEST(1171) { CHECK(RunTest(1171)); }
    TEST(1172) { CHECK(RunTest(1172)); }
    TEST(1173) { CHECK(RunTest(1173)); }
    TEST(1174) { CHECK(RunTest(1174)); }
    TEST(1175) { CHECK(RunTest(1175)); }
    TEST(1176) { CHECK(RunTest(1176)); }
    TEST(1177) { CHECK(RunTest(1177)); }
    TEST(1178) { CHECK(RunTest(1178)); }
    TEST(1179) { CHECK(RunTest(1179)); }
    TEST(1180) { CHECK(RunTest(1180)); }
    TEST(1181) { CHECK(RunTest(1181)); }
    TEST(1182) { CHECK(RunTest(1182)); }
    TEST(1183) { CHECK(RunTest(1183)); }
    TEST(1184) { CHECK(RunTest(1184)); }
    TEST(1185) { CHECK(RunTest(1185)); }
    TEST(1186) { CHECK(RunTest(1186)); }
    TEST(1187) { CHECK(RunTest(1187)); }
    TEST(1188) { CHECK(RunTest(1188)); }
    TEST(1189) { CHECK(RunTest(1189)); }
    TEST(1190) { CHECK(RunTest(1190)); }
    TEST(1191) { CHECK(RunTest(1191)); }
    TEST(1192) { CHECK(RunTest(1192)); }
    TEST(1193) { CHECK(RunTest(1193)); }
    TEST(1194) { CHECK(RunTest(1194)); }
    TEST(1195) { CHECK(RunTest(1195)); }
    TEST(1196) { CHECK(RunTest(1196)); }
    TEST(1197) { CHECK(RunTest(1197)); }
    //TEST(1198) { CHECK(RunTest(1198)); } // ASTNode destructor throws error.
    TEST(1199) { CHECK(RunTest(1199)); }
    TEST(1200) { CHECK(RunTest(1200)); }
    TEST(1201) { CHECK(RunTest(1201)); }
    TEST(1202) { CHECK(RunTest(1202)); }
    TEST(1203) { CHECK(RunTest(1203)); }
    TEST(1204) { CHECK(RunTest(1204)); }
    TEST(1205) { CHECK(RunTest(1205)); }
    TEST(1206) { CHECK(RunTest(1206)); }
    TEST(1207) { CHECK(RunTest(1207)); }
    //TEST(1208) { CHECK(RunTest(1208)); } // ASTNode destructor throws error.
    TEST(1209) { CHECK(RunTest(1209)); }
    TEST(1210) { CHECK(RunTest(1210)); }
    TEST(1211) { CHECK(RunTest(1211)); }
    TEST(1212) { CHECK(RunTest(1212)); }
    TEST(1213) { CHECK(RunTest(1213)); }
    TEST(1214) { CHECK(RunTest(1214)); }
    TEST(1215) { CHECK(RunTest(1215)); }
    TEST(1216) { CHECK(RunTest(1216)); }
    TEST(1217) { CHECK(RunTest(1217)); }
    TEST(1218) { CHECK(RunTest(1218)); }
    TEST(1219) { CHECK(RunTest(1219)); }
    TEST(1220) { CHECK(RunTest(1220)); }
    TEST(1221) { CHECK(RunTest(1221)); }
    TEST(1222) { CHECK(RunTest(1222)); }
    TEST(1223) { CHECK(RunTest(1223)); }
    TEST(1224) { CHECK(RunTest(1224)); }
    TEST(1225) { CHECK(RunTest(1225)); }
    TEST(1226) { CHECK(RunTest(1226)); }
    TEST(1227) { CHECK(RunTest(1227)); }
    TEST(1228) { CHECK(RunTest(1228)); }
    TEST(1229) { CHECK(RunTest(1229)); }
    TEST(1230) { CHECK(RunTest(1230)); }
    TEST(1231) { CHECK(RunTest(1231)); }
    TEST(1232) { CHECK(RunTest(1232)); }
    TEST(1233) { CHECK(RunTest(1233)); }
    //TEST(1234) { CHECK(RunTest(1234)); } // Missing ASTNode throw.
    //TEST(1235) { CHECK(RunTest(1235)); } // Missing ASTNode throw.
    //TEST(1236) { CHECK(RunTest(1236)); } // Missing ASTNode throw.
    TEST(1237) { CHECK(RunTest(1237)); }
    //TEST(1238) { CHECK(RunTest(1238)); } // Unknown throw.
    //TEST(1239) { CHECK(RunTest(1239)); } // Missing ASTNode throw.
    TEST(1240) { CHECK(RunTest(1240)); }
    //TEST(1241) { CHECK(RunTest(1241)); } // Unknown throw.
    //TEST(1242) { CHECK(RunTest(1242)); } // Unknown throw.
    TEST(1243) { CHECK(RunTest(1243)); }
    //TEST(1244) { CHECK(RunTest(1244)); } // Unknown throw.
    TEST(1245) { CHECK(RunTest(1245)); }
    TEST(1246) { CHECK(RunTest(1246)); }
    TEST(1247) { CHECK(RunTest(1247)); }
    //TEST(1248) { CHECK(RunTest(1248)); } // Failure to load model
    //TEST(1249) { CHECK(RunTest(1249)); } // Failure to load model
    //TEST(1250) { CHECK(RunTest(1250)); } // Failure to load model
    //TEST(1251) { CHECK(RunTest(1251)); } // Failure to load model
    //TEST(1252) { CHECK(RunTest(1252)); } // Failure to load model
    //TEST(1253) { CHECK(RunTest(1253)); } // Failure to load model
    //TEST(1254) { CHECK(RunTest(1254)); } // Failure to load model
    //TEST(1255) { CHECK(RunTest(1255)); } // Failure to load model
    //TEST(1256) { CHECK(RunTest(1256)); } // Failure to load model
    //TEST(1257) { CHECK(RunTest(1257)); } // Unknown throw
    //TEST(1258) { CHECK(RunTest(1258)); } // Failure to load model
    //TEST(1259) { CHECK(RunTest(1259)); } // Failure to load model
    //TEST(1260) { CHECK(RunTest(1260)); } // Failure to load model
    //TEST(1261) { CHECK(RunTest(1261)); } // Failure to load model
    //TEST(1262) { CHECK(RunTest(1262)); } // Failure to load model
    //TEST(1263) { CHECK(RunTest(1263)); } // Failure to load model
    //TEST(1264) { CHECK(RunTest(1264)); } // Failure to load model
    //TEST(1265) { CHECK(RunTest(1265)); } // Failure to load model
    //TEST(1266) { CHECK(RunTest(1266)); } // Failure to load model
    //TEST(1267) { CHECK(RunTest(1267)); } // Failure to load model
    //TEST(1268) { CHECK(RunTest(1268)); } // Unknown throw
    //TEST(1269) { CHECK(RunTest(1269)); } // Failure to load model
    //TEST(1270) { CHECK(RunTest(1270)); } // Failure to load model
    TEST(1271) { CHECK(RunTest(1271)); }
    TEST(1272) { CHECK(RunTest(1272)); }
    TEST(1273) { CHECK(RunTest(1273)); }
    TEST(1274) { CHECK(RunTest(1274)); }
    TEST(1275) { CHECK(RunTest(1275)); }
    TEST(1276) { CHECK(RunTest(1276)); }
    TEST(1277) { CHECK(RunTest(1277)); }
    TEST(1278) { CHECK(RunTest(1278)); }
    TEST(1279) { CHECK(RunTest(1279)); }
    TEST(1280) { CHECK(RunTest(1280)); }
    TEST(1281) { CHECK(RunTest(1281)); }
    //TEST(1282) { CHECK(RunTest(1282)); } // Throw; bad pointer.
    //TEST(1283) { CHECK(RunTest(1283)); } // Throw; bad pointer.
    //TEST(1284) { CHECK(RunTest(1284)); } // Throw; invalid cast.
    //TEST(1285) { CHECK(RunTest(1285)); } // Throw; bad pointer.
    //TEST(1286) { CHECK(RunTest(1286)); } //Throw; corrupt generated function (!)
    TEST(1287) { CHECK(RunTest(1287)); }
    //TEST(1288) { CHECK(RunTest(1288)); } // Throw; bad pointer.
    //TEST(1289) { CHECK(RunTest(1289)); } // Throw; bad pointer.
    //TEST(1290) { CHECK(RunTest(1290)); } // Throw; bad pointer.
    //TEST(1291) { CHECK(RunTest(1291)); } //Throw; corrupt generated function (!)
    //TEST(1292) { CHECK(RunTest(1292)); } // Throws because uninitialized value
    //TEST(1293) { CHECK(RunTest(1293)); } //Unknown throw
    //TEST(1294) { CHECK(RunTest(1294)); } // Failure to load model
    //TEST(1295) { CHECK(RunTest(1295)); } // Failure to load model
    //TEST(1296) { CHECK(RunTest(1296)); } // Failure to load model
    //TEST(1297) { CHECK(RunTest(1297)); } // Failure to load model
    //TEST(1298) { CHECK(RunTest(1298)); } // Failure to load model
    //TEST(1299) { CHECK(RunTest(1299)); } // Unknown throw
    TEST(1300) { CHECK(RunTest(1300)); }
    TEST(1301) { CHECK(RunTest(1301)); }
    TEST(1302) { CHECK(RunTest(1302)); }
    TEST(1303) { CHECK(RunTest(1303)); }
    TEST(1304) { CHECK(RunTest(1304)); }
    TEST(1305) { CHECK(RunTest(1305)); }
    TEST(1306) { CHECK(RunTest(1306)); }
    TEST(1307) { CHECK(RunTest(1307)); }
    TEST(1308) { CHECK(RunTest(1308)); }
    TEST(1309) { CHECK(RunTest(1309)); }
    TEST(1310) { CHECK(RunTest(1310)); }
    TEST(1311) { CHECK(RunTest(1311)); }
    TEST(1312) { CHECK(RunTest(1312)); }
    TEST(1313) { CHECK(RunTest(1313)); }
    TEST(1314) { CHECK(RunTest(1314)); }
    TEST(1315) { CHECK(RunTest(1315)); }
    TEST(1316) { CHECK(RunTest(1316)); }
    TEST(1317) { CHECK(RunTest(1317)); }
    TEST(1318) { CHECK(RunTest(1318)); }
    TEST(1319) { CHECK(RunTest(1319)); }
    TEST(1320) { CHECK(RunTest(1320)); }
    //TEST(1321) { CHECK(RunTest(1321)); } // Failure to load model
    //TEST(1322) { CHECK(RunTest(1322)); } // Failure to load model
    TEST(1323) { CHECK(RunTest(1323)); }
    TEST(1324) { CHECK(RunTest(1324)); }
    TEST(1325) { CHECK(RunTest(1325)); }
    TEST(1326) { CHECK(RunTest(1326)); }
    TEST(1327) { CHECK(RunTest(1327)); }
    TEST(1328) { CHECK(RunTest(1328)); }
    TEST(1329) { CHECK(RunTest(1329)); }
    TEST(1330) { CHECK(RunTest(1330)); }
    TEST(1331) { CHECK(RunTest(1331)); }
    TEST(1332) { CHECK(RunTest(1332)); }
    TEST(1333) { CHECK(RunTest(1333)); }
    TEST(1334) { CHECK(RunTest(1334)); }
    TEST(1335) { CHECK(RunTest(1335)); }
    TEST(1336) { CHECK(RunTest(1336)); }
    TEST(1337) { CHECK(RunTest(1337)); }
    TEST(1338) { CHECK(RunTest(1338)); }
    TEST(1339) { CHECK(RunTest(1339)); }
    TEST(1340) { CHECK(RunTest(1340)); }
    TEST(1341) { CHECK(RunTest(1341)); }
    TEST(1342) { CHECK(RunTest(1342)); }
    TEST(1343) { CHECK(RunTest(1343)); }
    TEST(1344) { CHECK(RunTest(1344)); }
    TEST(1345) { CHECK(RunTest(1345)); }
    TEST(1346) { CHECK(RunTest(1346)); }
    TEST(1347) { CHECK(RunTest(1347)); }
    TEST(1348) { CHECK(RunTest(1348)); }
    TEST(1349) { CHECK(RunTest(1349)); }
    //TEST(1350) { CHECK(RunTest(1350)); } //Fail due to l3v2 flattening
    TEST(1351) { CHECK(RunTest(1351)); }
    TEST(1352) { CHECK(RunTest(1352)); }
    TEST(1353) { CHECK(RunTest(1353)); }
    TEST(1354) { CHECK(RunTest(1354)); }
    TEST(1355) { CHECK(RunTest(1355)); }
    TEST(1356) { CHECK(RunTest(1356)); }
    TEST(1357) { CHECK(RunTest(1357)); }
    TEST(1358) { CHECK(RunTest(1358)); }
    //TEST(1359) { CHECK(RunTest(1359)); } //Fail due to l3v2 flattening
    TEST(1360) { CHECK(RunTest(1360)); }
    TEST(1361) { CHECK(RunTest(1361)); }
    TEST(1362) { CHECK(RunTest(1362)); }
    TEST(1363) { CHECK(RunTest(1363)); }
    TEST(1364) { CHECK(RunTest(1364)); }
    TEST(1365) { CHECK(RunTest(1365)); }
    TEST(1366) { CHECK(RunTest(1366)); }
    TEST(1367) { CHECK(RunTest(1367)); }
    //TEST(1368) { CHECK(RunTest(1368)); } //Fail due to l3v2 flattening
    TEST(1369) { CHECK(RunTest(1369)); }
    TEST(1370) { CHECK(RunTest(1370)); }
    TEST(1371) { CHECK(RunTest(1371)); }
    TEST(1372) { CHECK(RunTest(1372)); }
    TEST(1373) { CHECK(RunTest(1373)); }
    TEST(1374) { CHECK(RunTest(1374)); }
    TEST(1375) { CHECK(RunTest(1375)); }
    TEST(1376) { CHECK(RunTest(1376)); }
    //TEST(1377) { CHECK(RunTest(1377)); } //Fail due to l3v2 flattening
    TEST(1378) { CHECK(RunTest(1378)); }
    TEST(1379) { CHECK(RunTest(1379)); }
    TEST(1380) { CHECK(RunTest(1380)); }
    TEST(1381) { CHECK(RunTest(1381)); }
    TEST(1382) { CHECK(RunTest(1382)); }
    TEST(1383) { CHECK(RunTest(1383)); }
    TEST(1384) { CHECK(RunTest(1384)); }
    TEST(1385) { CHECK(RunTest(1385)); }
    //TEST(1386) { CHECK(RunTest(1386)); } //Fail due to l3v2 flattening
    TEST(1387) { CHECK(RunTest(1387)); }
    TEST(1388) { CHECK(RunTest(1388)); }
    TEST(1389) { CHECK(RunTest(1389)); }
    TEST(1390) { CHECK(RunTest(1390)); }
    TEST(1391) { CHECK(RunTest(1391)); }
    TEST(1392) { CHECK(RunTest(1392)); }
    TEST(1393) { CHECK(RunTest(1393)); }
    TEST(1394) { CHECK(RunTest(1394)); }
    TEST(1395) { CHECK(RunTest(1395)); }
    TEST(1396) { CHECK(RunTest(1396)); }
    TEST(1397) { CHECK(RunTest(1397)); }
    //TEST(1398) { CHECK(RunTest(1398)); } // Unknown throw
    TEST(1399) { CHECK(RunTest(1399)); }
    //TEST(1400) { CHECK(RunTest(1400)); } // Failure to load model.
    //TEST(1401) { CHECK(RunTest(1401)); } // Failure to load model.
    //TEST(1402) { CHECK(RunTest(1402)); } // Failure to load model.
    //TEST(1403) { CHECK(RunTest(1403)); } // Failure to load model.
    TEST(1404) { CHECK(RunTest(1404)); }
    //TEST(1405) { CHECK(RunTest(1405)); } // Failure to load model.
    //TEST(1406) { CHECK(RunTest(1406)); } // Failure to load model.
    TEST(1407) { CHECK(RunTest(1407)); }
    //TEST(1408) { CHECK(RunTest(1408)); } // Failure to load model.
    //TEST(1409) { CHECK(RunTest(1409)); } // Failure to load model.
    TEST(1410) { CHECK(RunTest(1410)); }
    TEST(1411) { CHECK(RunTest(1411)); }
    TEST(1412) { CHECK(RunTest(1412)); }
    TEST(1413) { CHECK(RunTest(1413)); }
    TEST(1414) { CHECK(RunTest(1414)); }
    TEST(1415) { CHECK(RunTest(1415)); }
    ////TEST(1416) { CHECK(RunTest(1416)); } //Unknown throw
    TEST(1417) { CHECK(RunTest(1417)); }
    TEST(1418) { CHECK(RunTest(1418)); }
    //TEST(1419) { CHECK(RunTest(1419)); } //Unknown throw
    TEST(1420) { CHECK(RunTest(1420)); }
    TEST(1421) { CHECK(RunTest(1421)); }
    TEST(1422) { CHECK(RunTest(1422)); }
    TEST(1423) { CHECK(RunTest(1423)); }
    TEST(1424) { CHECK(RunTest(1424)); }
    TEST(1425) { CHECK(RunTest(1425)); }
    TEST(1426) { CHECK(RunTest(1426)); }
    TEST(1427) { CHECK(RunTest(1427)); }
    TEST(1428) { CHECK(RunTest(1428)); }
    TEST(1429) { CHECK(RunTest(1429)); }
    TEST(1430) { CHECK(RunTest(1430)); }
    TEST(1431) { CHECK(RunTest(1431)); }
    TEST(1432) { CHECK(RunTest(1432)); }
    TEST(1433) { CHECK(RunTest(1433)); }
    TEST(1434) { CHECK(RunTest(1434)); }
    TEST(1435) { CHECK(RunTest(1435)); }
    TEST(1436) { CHECK(RunTest(1436)); }
    TEST(1437) { CHECK(RunTest(1437)); }
    TEST(1438) { CHECK(RunTest(1438)); }
    TEST(1439) { CHECK(RunTest(1439)); }
    TEST(1440) { CHECK(RunTest(1440)); }
    TEST(1441) { CHECK(RunTest(1441)); }
    TEST(1442) { CHECK(RunTest(1442)); }
    TEST(1443) { CHECK(RunTest(1443)); }
    //TEST(1444) { CHECK(RunTest(1444)); } // Failure to load model.
    //TEST(1445) { CHECK(RunTest(1445)); } // Failure to load model.
    //TEST(1446) { CHECK(RunTest(1446)); } // Incorrect interpretation (species references)
    //TEST(1447) { CHECK(RunTest(1447)); } // Failure to load model.
    //TEST(1448) { CHECK(RunTest(1448)); } // Unknown throw
    //TEST(1449) { CHECK(RunTest(1449)); } // Unknown throw
    //TEST(1450) { CHECK(RunTest(1450)); } // Unknown throw
    //TEST(1451) { CHECK(RunTest(1451)); } // Incorrect interpretation
    //TEST(1452) { CHECK(RunTest(1452)); } // Unknown throw
    //TEST(1453) { CHECK(RunTest(1453)); } // Unknown throw
    //TEST(1454) { CHECK(RunTest(1454)); } // Unknown throw
    //TEST(1455) { CHECK(RunTest(1455)); } // Failure to load model.
    //TEST(1456) { CHECK(RunTest(1456)); } // Failure to load model.
    //TEST(1457) { CHECK(RunTest(1457)); } // Failure to load model.
    //TEST(1458) { CHECK(RunTest(1458)); } // Failure to load model.
    //TEST(1459) { CHECK(RunTest(1459)); } // Failure to load model.
    //TEST(1460) { CHECK(RunTest(1460)); } // Failure to load model.
    //TEST(1461) { CHECK(RunTest(1461)); } // Failure to load model.
    //TEST(1462) { CHECK(RunTest(1462)); } // Failure to load model.
    //TEST(1463) { CHECK(RunTest(1463)); } // Failure to load model.
    //TEST(1464) { CHECK(RunTest(1464)); } // Failure to load model.
    //TEST(1465) { CHECK(RunTest(1465)); } // Unknown throw.
    TEST(1466) { CHECK(RunTest(1466)); }
    TEST(1467) { CHECK(RunTest(1467)); }
    TEST(1468) { CHECK(RunTest(1468)); }
    TEST(1469) { CHECK(RunTest(1469)); }
    TEST(1470) { CHECK(RunTest(1470)); }
    //TEST(1471) { CHECK(RunTest(1471)); } //Fail due to l3v2 flattening
    //TEST(1472) { CHECK(RunTest(1472)); } //Fail due to l3v2 flattening
    //TEST(1473) { CHECK(RunTest(1473)); } //Fail due to l3v2 flattening
    TEST(1474) { CHECK(RunTest(1474)); }
    //TEST(1475) { CHECK(RunTest(1475)); } //Fail due to l3v2 flattening
    //TEST(1476) { CHECK(RunTest(1476)); } //Fail due to l3v2 flattening
    //TEST(1477) { CHECK(RunTest(1477)); } //Fail due to l3v2 flattening
    //TEST(1478) { CHECK(RunTest(1478)); } // Unknown throw.
    //TEST(1479) { CHECK(RunTest(1479)); } // Throws because uninitialized value
    TEST(1480) { CHECK(RunTest(1480)); }
    TEST(1481) { CHECK(RunTest(1481)); }
    //TEST(1482) { CHECK(RunTest(1482)); } // Throws because uninitialized value
    //TEST(1483) { CHECK(RunTest(1483)); } // Throws because uninitialized value
    //TEST(1484) { CHECK(RunTest(1484)); } // Throws because uninitialized value
    TEST(1485) { CHECK(RunTest(1485)); }
    TEST(1486) { CHECK(RunTest(1486)); }
    TEST(1487) { CHECK(RunTest(1487)); }
    //TEST(1488) { CHECK(RunTest(1488)); } // Incorrect interpretation: odd mathml in functions
    TEST(1489) { CHECK(RunTest(1489)); }
    TEST(1490) { CHECK(RunTest(1490)); }
    TEST(1491) { CHECK(RunTest(1491)); }
    TEST(1492) { CHECK(RunTest(1492)); }
    TEST(1493) { CHECK(RunTest(1493)); }
    TEST(1494) { CHECK(RunTest(1494)); }
    TEST(1495) { CHECK(RunTest(1495)); }
    TEST(1496) { CHECK(RunTest(1496)); }
    TEST(1497) { CHECK(RunTest(1497)); }
    //TEST(1498) { CHECK(RunTest(1498)); } // Unknown throw.
    //TEST(1499) { CHECK(RunTest(1499)); } // Throws because uninitialized value
    //TEST(1500) { CHECK(RunTest(1500)); } // Throws because uninitialized value
    //TEST(1501) { CHECK(RunTest(1501)); } // Throws because uninitialized value
    //TEST(1502) { CHECK(RunTest(1502)); } // Throws because uninitialized value
    //TEST(1503) { CHECK(RunTest(1503)); } // Throws because uninitialized value
    //TEST(1504) { CHECK(RunTest(1504)); } // Unknown throw
    //TEST(1505) { CHECK(RunTest(1505)); } // Unknown throw
    //TEST(1506) { CHECK(RunTest(1506)); } // Unknown throw
    //TEST(1507) { CHECK(RunTest(1507)); } // Unknown throw
    //TEST(1508) { CHECK(RunTest(1508)); } // Unknown throw
    //TEST(1509) { CHECK(RunTest(1509)); } // Unknown throw
    //TEST(1510) { CHECK(RunTest(1510)); } // Unknown throw
    //TEST(1511) { CHECK(RunTest(1511)); } // Unknown throw
    //TEST(1512) { CHECK(RunTest(1512)); } // Unknown throw
    //TEST(1513) { CHECK(RunTest(1513)); } // Unknown throw
    //TEST(1514) { CHECK(RunTest(1514)); } // Unknown throw
    TEST(1515) { CHECK(RunTest(1515)); }
    TEST(1516) { CHECK(RunTest(1516)); }
    //TEST(1517) { CHECK(RunTest(1517)); } // Incorrect mathml interpretation
    TEST(1518) { CHECK(RunTest(1518)); }
    TEST(1519) { CHECK(RunTest(1519)); }
    TEST(1520) { CHECK(RunTest(1520)); }
    TEST(1521) { CHECK(RunTest(1521)); }
    TEST(1522) { CHECK(RunTest(1522)); }
    TEST(1523) { CHECK(RunTest(1523)); }
    TEST(1524) { CHECK(RunTest(1524)); }
    //TEST(1525) { CHECK(RunTest(1525)); } // Unknown throw
    //TEST(1526) { CHECK(RunTest(1526)); } // Unknown throw
    //TEST(1527) { CHECK(RunTest(1527)); } // Unknown throw
    //TEST(1528) { CHECK(RunTest(1528)); } // Failure to load model.
    //TEST(1529) { CHECK(RunTest(1529)); } // Failure to load model.
    TEST(1530) { CHECK(RunTest(1530)); }
    TEST(1531) { CHECK(RunTest(1531)); }
    TEST(1532) { CHECK(RunTest(1532)); }
    TEST(1533) { CHECK(RunTest(1533)); }
    TEST(1534) { CHECK(RunTest(1534)); }
    TEST(1535) { CHECK(RunTest(1535)); }
    TEST(1536) { CHECK(RunTest(1536)); }
    TEST(1537) { CHECK(RunTest(1537)); }
    TEST(1538) { CHECK(RunTest(1538)); }
    TEST(1539) { CHECK(RunTest(1539)); }
    //TEST(1540) { CHECK(RunTest(1540)); } // Failure to load model.
    //TEST(1541) { CHECK(RunTest(1541)); } // Failure to load model.
    //TEST(1542) { CHECK(RunTest(1542)); } // Failure to load model.
    //TEST(1543) { CHECK(RunTest(1543)); } // Failure to load model.
    TEST(1544) { CHECK(RunTest(1544)); }
    TEST(1545) { CHECK(RunTest(1545)); }
    TEST(1546) { CHECK(RunTest(1546)); }
    TEST(1547) { CHECK(RunTest(1547)); }
    TEST(1548) { CHECK(RunTest(1548)); }
    TEST(1549) { CHECK(RunTest(1549)); }
    TEST(1550) { CHECK(RunTest(1550)); }
    TEST(1551) { CHECK(RunTest(1551)); }
    //TEST(1552) { CHECK(RunTest(1552)); } // Unknown throw.
    //TEST(1553) { CHECK(RunTest(1553)); } // Unknown throw.
    //TEST(1554) { CHECK(RunTest(1554)); } // Unknown throw.
    TEST(1555) { CHECK(RunTest(1555)); }
    //TEST(1556) { CHECK(RunTest(1556)); } // Unknown throw.
    TEST(1557) { CHECK(RunTest(1557)); }
    TEST(1558) { CHECK(RunTest(1558)); }
    TEST(1559) { CHECK(RunTest(1559)); }
    TEST(1560) { CHECK(RunTest(1560)); }
    TEST(1561) { CHECK(RunTest(1561)); }
    TEST(1562) { CHECK(RunTest(1562)); }
    TEST(1563) { CHECK(RunTest(1563)); }
    TEST(1564) { CHECK(RunTest(1564)); }
    TEST(1565) { CHECK(RunTest(1565)); }
    TEST(1566) { CHECK(RunTest(1566)); }
    //TEST(1567) { CHECK(RunTest(1567)); } // Throws because uninitialized value
    TEST(1568) { CHECK(RunTest(1568)); }
    TEST(1569) { CHECK(RunTest(1569)); }
    TEST(1570) { CHECK(RunTest(1570)); }
    //TEST(1571) { CHECK(RunTest(1571)); } // Unknown throw.
    TEST(1572) { CHECK(RunTest(1572)); }
    TEST(1573) { CHECK(RunTest(1573)); }
    //TEST(1574) { CHECK(RunTest(1574)); } // Unknown throw.
    //TEST(1575) { CHECK(RunTest(1575)); } // Throws because uninitialized value
    //TEST(1576) { CHECK(RunTest(1576)); } // Throws because uninitialized value
    //TEST(1577) { CHECK(RunTest(1577)); } // Throws because uninitialized value
    TEST(1578) { CHECK(RunTest(1578)); }    
    //TEST(1579) { CHECK(RunTest(1579)); } // Throws because uninitialized value
    TEST(1580) { CHECK(RunTest(1580)); }
    //TEST(1581) { CHECK(RunTest(1581)); } // Fail l2v1, pass l2v5
    TEST(1582) { CHECK(RunTest(1582)); }
    TEST(1583) { CHECK(RunTest(1583)); }
    TEST(1584) { CHECK(RunTest(1584)); }
    //TEST(1585) { CHECK(RunTest(1585)); } // Fail l2v1, pass l2v5
    TEST(1586) { CHECK(RunTest(1586)); }
    TEST(1587) { CHECK(RunTest(1587)); }
    TEST(1588) { CHECK(RunTest(1588)); }
    //TEST(1589) { CHECK(RunTest(1589)); } // Throws because uninitialized value
    TEST(1590) { CHECK(RunTest(1590)); }
    TEST(1591) { CHECK(RunTest(1591)); }
    TEST(1592) { CHECK(RunTest(1592)); }
    TEST(1593) { CHECK(RunTest(1593)); }
    TEST(1594) { CHECK(RunTest(1594)); }
    TEST(1595) { CHECK(RunTest(1595)); }
    TEST(1596) { CHECK(RunTest(1596)); }
    TEST(1597) { CHECK(RunTest(1597)); }
    TEST(1598) { CHECK(RunTest(1598)); }
    TEST(1599) { CHECK(RunTest(1599)); }
    TEST(1600) { CHECK(RunTest(1600)); }
    TEST(1601) { CHECK(RunTest(1601)); }
    TEST(1602) { CHECK(RunTest(1602)); }
    TEST(1603) { CHECK(RunTest(1603)); }
    TEST(1604) { CHECK(RunTest(1604)); }
    TEST(1605) { CHECK(RunTest(1605)); }
    TEST(1606) { CHECK(RunTest(1606)); }
    TEST(1607) { CHECK(RunTest(1607)); }
    TEST(1608) { CHECK(RunTest(1608)); }
    TEST(1609) { CHECK(RunTest(1609)); }
    TEST(1610) { CHECK(RunTest(1610)); }
    TEST(1611) { CHECK(RunTest(1611)); }
    TEST(1612) { CHECK(RunTest(1612)); }
    TEST(1613) { CHECK(RunTest(1613)); }
    TEST(1614) { CHECK(RunTest(1614)); }
    TEST(1615) { CHECK(RunTest(1615)); }
    TEST(1616) { CHECK(RunTest(1616)); }
    TEST(1617) { CHECK(RunTest(1617)); }
    TEST(1618) { CHECK(RunTest(1618)); }
    TEST(1619) { CHECK(RunTest(1619)); }
    TEST(1620) { CHECK(RunTest(1620)); }
    TEST(1621) { CHECK(RunTest(1621)); }
    TEST(1622) { CHECK(RunTest(1622)); }
    TEST(1623) { CHECK(RunTest(1623)); }
    TEST(1624) { CHECK(RunTest(1624)); }
    TEST(1625) { CHECK(RunTest(1625)); }
    //TEST(1626) { CHECK(RunTest(1626)); } // Unknown throw.
    //TEST(1627) { CHECK(RunTest(1627)); } // Unknown throw.
    //TEST(1628) { CHECK(RunTest(1628)); } // Throw; AST unknown pointer.
    //TEST(1629) { CHECK(RunTest(1629)); } // Throw; AST unknown pointer.
    //TEST(1630) { CHECK(RunTest(1630)); } // Throw; AST unknown pointer.
    TEST(1631) { CHECK(RunTest(1631)); }
    TEST(1632) { CHECK(RunTest(1632)); }
    TEST(1633) { CHECK(RunTest(1633)); }
    TEST(1634) { CHECK(RunTest(1634)); }
    //TEST(1635) { CHECK(RunTest(1635)); } // Unknown throw.
    //TEST(1636) { CHECK(RunTest(1636)); } // l2v1 passes, l2v5 throws (!)
    TEST(1637) { CHECK(RunTest(1637)); }
    TEST(1638) { CHECK(RunTest(1638)); }
    TEST(1639) { CHECK(RunTest(1639)); }
    TEST(1640) { CHECK(RunTest(1640)); }
    TEST(1641) { CHECK(RunTest(1641)); }
    TEST(1642) { CHECK(RunTest(1642)); }
    TEST(1643) { CHECK(RunTest(1643)); }
    TEST(1644) { CHECK(RunTest(1644)); }
    TEST(1645) { CHECK(RunTest(1645)); }
    TEST(1646) { CHECK(RunTest(1646)); }
    TEST(1647) { CHECK(RunTest(1647)); }
    TEST(1648) { CHECK(RunTest(1648)); }
    TEST(1649) { CHECK(RunTest(1649)); }
    TEST(1650) { CHECK(RunTest(1650)); }
    TEST(1651) { CHECK(RunTest(1651)); }
    TEST(1652) { CHECK(RunTest(1652)); }
    TEST(1653) { CHECK(RunTest(1653)); }
    TEST(1654) { CHECK(RunTest(1654)); }
    TEST(1655) { CHECK(RunTest(1655)); }
    TEST(1656) { CHECK(RunTest(1656)); }
    //TEST(1657) { CHECK(RunTest(1657)); } // Throw; AST unknown pointer.
    TEST(1658) { CHECK(RunTest(1658)); }
    TEST(1659) { CHECK(RunTest(1659)); }
    TEST(1660) { CHECK(RunTest(1660)); }
    TEST(1661) { CHECK(RunTest(1661)); }
    TEST(1662) { CHECK(RunTest(1662)); }
    TEST(1663) { CHECK(RunTest(1663)); }
    TEST(1664) { CHECK(RunTest(1664)); }
    TEST(1665) { CHECK(RunTest(1665)); }
    TEST(1666) { CHECK(RunTest(1666)); }
    TEST(1667) { CHECK(RunTest(1667)); }
    TEST(1668) { CHECK(RunTest(1668)); }
    TEST(1669) { CHECK(RunTest(1669)); }
    TEST(1670) { CHECK(RunTest(1670)); }
    TEST(1671) { CHECK(RunTest(1671)); }
    TEST(1672) { CHECK(RunTest(1672)); }
    TEST(1673) { CHECK(RunTest(1673)); }
    TEST(1674) { CHECK(RunTest(1674)); }
    TEST(1675) { CHECK(RunTest(1675)); }
    TEST(1676) { CHECK(RunTest(1676)); }
    TEST(1677) { CHECK(RunTest(1677)); }
    TEST(1678) { CHECK(RunTest(1678)); }
    TEST(1679) { CHECK(RunTest(1679)); }
    TEST(1680) { CHECK(RunTest(1680)); }
    TEST(1681) { CHECK(RunTest(1681)); }
    TEST(1682) { CHECK(RunTest(1682)); }
    TEST(1683) { CHECK(RunTest(1683)); }
    TEST(1684) { CHECK(RunTest(1684)); }
    TEST(1685) { CHECK(RunTest(1685)); }
    TEST(1686) { CHECK(RunTest(1686)); }
    TEST(1687) { CHECK(RunTest(1687)); }
    TEST(1688) { CHECK(RunTest(1688)); }
    TEST(1689) { CHECK(RunTest(1689)); }
    TEST(1690) { CHECK(RunTest(1690)); }
    TEST(1691) { CHECK(RunTest(1691)); }
    TEST(1692) { CHECK(RunTest(1692)); }
    TEST(1693) { CHECK(RunTest(1693)); }
    TEST(1694) { CHECK(RunTest(1694)); }
    TEST(1695) { CHECK(RunTest(1695)); }
    TEST(1696) { CHECK(RunTest(1696)); }
    TEST(1697) { CHECK(RunTest(1697)); }
    TEST(1698) { CHECK(RunTest(1698)); }
    TEST(1699) { CHECK(RunTest(1699)); }
    TEST(1700) { CHECK(RunTest(1700)); }
    TEST(1701) { CHECK(RunTest(1701)); }
    TEST(1702) { CHECK(RunTest(1702)); }
    TEST(1703) { CHECK(RunTest(1703)); }
    TEST(1704) { CHECK(RunTest(1704)); }
    TEST(1705) { CHECK(RunTest(1705)); }
    TEST(1706) { CHECK(RunTest(1706)); }
    TEST(1707) { CHECK(RunTest(1707)); }
    TEST(1708) { CHECK(RunTest(1708)); }
    TEST(1709) { CHECK(RunTest(1709)); }
    TEST(1710) { CHECK(RunTest(1710)); }
    TEST(1711) { CHECK(RunTest(1711)); }
    TEST(1712) { CHECK(RunTest(1712)); }
    TEST(1713) { CHECK(RunTest(1713)); }
    TEST(1714) { CHECK(RunTest(1714)); }
    TEST(1715) { CHECK(RunTest(1715)); }
    TEST(1716) { CHECK(RunTest(1716)); }
    TEST(1717) { CHECK(RunTest(1717)); }
    TEST(1718) { CHECK(RunTest(1718)); }
    TEST(1719) { CHECK(RunTest(1719)); }
    TEST(1720) { CHECK(RunTest(1720)); }
    TEST(1721) { CHECK(RunTest(1721)); }
    TEST(1722) { CHECK(RunTest(1722)); }
    TEST(1723) { CHECK(RunTest(1723)); }
    TEST(1724) { CHECK(RunTest(1724)); }
    TEST(1725) { CHECK(RunTest(1725)); }
    TEST(1726) { CHECK(RunTest(1726)); }
    //TEST(1727) { CHECK(RunTest(1727)); } // Unknown throw.
    //TEST(1728) { CHECK(RunTest(1728)); } // Unknown throw.
    //TEST(1729) { CHECK(RunTest(1729)); } // Unknown throw.
    TEST(1730) { CHECK(RunTest(1730)); }
    TEST(1731) { CHECK(RunTest(1731)); }
    TEST(1732) { CHECK(RunTest(1732)); }
    TEST(1733) { CHECK(RunTest(1733)); }
    TEST(1734) { CHECK(RunTest(1734)); }
    TEST(1735) { CHECK(RunTest(1735)); }
    //TEST(1736) { CHECK(RunTest(1736)); } // Failure to load model.
    //TEST(1737) { CHECK(RunTest(1737)); } // Failure to load model.
    //TEST(1738) { CHECK(RunTest(1738)); } // Failure to load model.
    TEST(1739) { CHECK(RunTest(1739)); }
    TEST(1740) { CHECK(RunTest(1740)); }
    TEST(1741) { CHECK(RunTest(1741)); }
    TEST(1742) { CHECK(RunTest(1742)); }
    //TEST(1743) { CHECK(RunTest(1743)); } // l2v1 fail; l2v5 pass
    TEST(1744) { CHECK(RunTest(1744)); }
    //TEST(1745) { CHECK(RunTest(1745)); } // l2v1 fail; l2v5 pass
    TEST(1746) { CHECK(RunTest(1746)); }
    TEST(1747) { CHECK(RunTest(1747)); }
    TEST(1748) { CHECK(RunTest(1748)); }
    //TEST(1749) { CHECK(RunTest(1749)); } // l2v1 fail; l2v5 pass
    TEST(1750) { CHECK(RunTest(1750)); }
    //TEST(1751) { CHECK(RunTest(1751)); } // l2v1 fail; l2v5 pass
    TEST(1752) { CHECK(RunTest(1752)); }
    TEST(1753) { CHECK(RunTest(1753)); }
    TEST(1754) { CHECK(RunTest(1754)); }
    TEST(1755) { CHECK(RunTest(1755)); }
    TEST(1756) { CHECK(RunTest(1756)); }
    TEST(1757) { CHECK(RunTest(1757)); }
    TEST(1758) { CHECK(RunTest(1758)); }
    TEST(1759) { CHECK(RunTest(1759)); }
    TEST(1760) { CHECK(RunTest(1760)); }
    TEST(1761) { CHECK(RunTest(1761)); }
    TEST(1762) { CHECK(RunTest(1762)); }
    TEST(1763) { CHECK(RunTest(1763)); }
    TEST(1764) { CHECK(RunTest(1764)); }
    TEST(1765) { CHECK(RunTest(1765)); }
    TEST(1766) { CHECK(RunTest(1766)); }
    TEST(1767) { CHECK(RunTest(1767)); }
    TEST(1768) { CHECK(RunTest(1768)); }
    TEST(1769) { CHECK(RunTest(1769)); }
    TEST(1770) { CHECK(RunTest(1770)); }
    TEST(1771) { CHECK(RunTest(1771)); }
    TEST(1772) { CHECK(RunTest(1772)); }
    TEST(1773) { CHECK(RunTest(1773)); }
    TEST(1774) { CHECK(RunTest(1774)); }
    TEST(1775) { CHECK(RunTest(1775)); }
    TEST(1776) { CHECK(RunTest(1776)); }
    TEST(1777) { CHECK(RunTest(1777)); }
    //TEST(1778) { CHECK(RunTest(1778)); } //Fail due to l3v2 flattening
    TEST(1779) { CHECK(RunTest(1779)); }
    //TEST(1780) { CHECK(RunTest(1780)); } // Incorrect interpretation.
    TEST(1781) { CHECK(RunTest(1781)); }
    TEST(1782) { CHECK(RunTest(1782)); }
    TEST(1783) { CHECK(RunTest(1783)); }
    TEST(1784) { CHECK(RunTest(1784)); }
    TEST(1785) { CHECK(RunTest(1785)); }
    TEST(1786) { CHECK(RunTest(1786)); }
    TEST(1787) { CHECK(RunTest(1787)); }
    TEST(1788) { CHECK(RunTest(1788)); }
    TEST(1789) { CHECK(RunTest(1789)); }
    TEST(1790) { CHECK(RunTest(1790)); }
    TEST(1791) { CHECK(RunTest(1791)); }
    TEST(1792) { CHECK(RunTest(1792)); }
    TEST(1793) { CHECK(RunTest(1793)); }
    TEST(1794) { CHECK(RunTest(1794)); }
    TEST(1795) { CHECK(RunTest(1795)); }
    TEST(1796) { CHECK(RunTest(1796)); }
    TEST(1797) { CHECK(RunTest(1797)); }
    TEST(1798) { CHECK(RunTest(1798)); }
    TEST(1799) { CHECK(RunTest(1799)); }
    TEST(1800) { CHECK(RunTest(1800)); }
    TEST(1801) { CHECK(RunTest(1801)); }
    TEST(1802) { CHECK(RunTest(1802)); }
    TEST(1803) { CHECK(RunTest(1803)); }
    TEST(1804) { CHECK(RunTest(1804)); }
    TEST(1805) { CHECK(RunTest(1805)); }
    TEST(1806) { CHECK(RunTest(1806)); }
    TEST(1807) { CHECK(RunTest(1807)); }
    TEST(1808) { CHECK(RunTest(1808)); }
    TEST(1809) { CHECK(RunTest(1809)); }
}

SUITE(SBML_BOOLEAN_DELAY)
{
    TEST(BOOLEAN_DELAY_1)
    {
        try
        {
            RoadRunner rri(joinPath(getParentFolder(getParentFolder(getParentFolder(gTSModelsPath))), "boolean_trigger.l3v2.xml"));
            rri.validateCurrentSBML();
            rri.getSimulateOptions().duration = 2;
            rri.simulate();
            CHECK(rri.getValue(rri.createSelection("x")) == 0.0);
            rri.getSimulateOptions().start = 0.1;
            rri.simulate();
            CHECK(rri.getValue(rri.createSelection("x")) == 3);
        }
        catch (std::exception& ex)
        {
            std::cout << "Exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }
    TEST(BOOLEAN_DELAY_2)
    {
        try
        {
            RoadRunner rri(joinPath(getParentFolder(getParentFolder(getParentFolder(gTSModelsPath))), "boolean_trigger_2.l3v2.xml"));
            rri.validateCurrentSBML();
            rri.getSimulateOptions().duration = 1;
            rri.simulate();
            CHECK(rri.getValue(rri.createSelection("x")) == 0.0);
            rri.getSimulateOptions().start = 0.1;
            rri.simulate();
            CHECK(rri.getValue(rri.createSelection("x")) == 3);
        }
        catch (std::exception& ex)
        {
            std::cout << "Exception: " << ex.what() << std::endl;
            CHECK(false);
        }
    }
}
bool RunTest(int caseNumber)
{
    //Run the first and last version of the file.
    string modelFileName, settingsFileName, descriptionFileName;
    vector<string> lvs;
    lvs.push_back("l1v2"); 
    lvs.push_back("l2v1"); 
    lvs.push_back("l2v2"); 
    lvs.push_back("l2v3"); 
    lvs.push_back("l2v4"); 
    lvs.push_back("l2v5"); 
    lvs.push_back("l3v1");
    lvs.push_back("l3v2");
    string modelFilePath(gTSModelsPath);
    string first = "";
    string last = "";
    for (size_t n = 0; n < lvs.size(); n++) {
        string lv = lvs[n];
        modelFilePath = gTSModelsPath; //Reset, since the subdir is added.
        createTestSuiteFileNameParts(caseNumber, "-sbml-" + lv + ".xml", modelFilePath, modelFileName, settingsFileName, descriptionFileName);
        ifstream ftest(modelFilePath + "/" + modelFileName);
        if (ftest.good()) {
            if (first.empty()) {
                first = lv;
            }
            else {
                last = lv;
            }
        }
    }
    bool ret = true;
    if (hasUnimplementedTags(modelFilePath + "/" + descriptionFileName)) {
        if (!first.empty()) {
            ret = CheckLoad(first, caseNumber);
        }
        else {
            Log(Logger::LOG_ERROR) << "No models found for test case" << caseNumber << endl;
            ret = false;
        }
        if (!last.empty()) {
            ret = CheckLoad(last, caseNumber) && ret;
        }
    }

    else {
        if (!first.empty()) {
            ret = RunTest(first, caseNumber);
        }
        else {
            Log(Logger::LOG_ERROR) << "No models found for test case" << caseNumber << endl;
            ret = false;
        }
        if (!last.empty()) {
            ret = RunTest(last, caseNumber) && ret;
        }
    }
    return ret;
}


bool RunTest(const string& version, int caseNumber)
{
    cerr << "Running Test:\t" << caseNumber << "\t" << version;

    RoadRunner rr(gCompiler, gTempFolder, gSupportCodeFolder);
    string dataOutputFolder = joinPath(gTempFolder, getTestSuiteSubFolderName(caseNumber));
    TestSuiteModelSimulation simulation(dataOutputFolder);
    try
    {
        LoadAndSimulate(version, caseNumber, rr, dataOutputFolder, simulation);

        //Write result
        if(!simulation.SaveResult())
        {
            //Failed to save data
            Log(Logger::LOG_ERROR)<<"Failed to save result";
            throw("Failed running simulation: Failed to save result");
        }

        if(!simulation.LoadReferenceData())
        {
            Log(Logger::LOG_ERROR)<<"Failed loading reference data";
            throw("Failed loading reference data");
        }

        simulation.CreateErrorData();
        bool result = simulation.Pass();
        simulation.SaveAllData();

        simulation.SaveModelAsXML(dataOutputFolder);

        cerr<<"\t"<< (result == true ? "PASS" : "FAIL")<<endl;
        return result;
     }
    catch(Exception& ex)
    {
        string error = ex.what();
        cerr << "\tFAIL" << endl;
        cerr<<"Case "<<caseNumber<<": Exception: "<<error<<endl;
        return false;
    }

}

bool CheckLoad(const string& version, int caseNumber)
{
    cerr << "Checking Test Loading:\t" << caseNumber << "\t" << version;

    RoadRunner rr(gCompiler, gTempFolder, gSupportCodeFolder);
    string dataOutputFolder = joinPath(gTempFolder, getTestSuiteSubFolderName(caseNumber));
    TestSuiteModelSimulation simulation(dataOutputFolder);

    try
    {
        LoadAndSimulate(version, caseNumber, rr, dataOutputFolder, simulation);

        //If we've gotten this far, rejoice!  roadrunner didn't crash, which is good enough.
        cerr << "\tPASS" << endl;
        return true;
    }
    catch (Exception& ex)
    {
        string error = ex.what();
        cerr << "\tFAIL" << endl;
        cerr << "Case " << caseNumber << ": Exception: " << error << endl;
        return false;
    }

}

void LoadAndSimulate(const string& version, int caseNumber, RoadRunner& rr, string& dataOutputFolder, TestSuiteModelSimulation& simulation)
{

    string dummy;
    string logFileName;

    rr.getIntegrator()->setValue("stiff", false);

    //Create log file name, e.g. 00001.log
    createTestSuiteFileNameParts(caseNumber, "_" + version + ".log", dummy, logFileName, dummy, dummy);

    if (!createFolder(dataOutputFolder))
    {
        throw(rr::Exception("Failed creating output folder for data output"));
    }

    //rr.reset();
    simulation.UseEngine(&rr);

    //Setup filenames and paths...
    string modelFilePath(gTSModelsPath);
    string modelFileName;
    string settingsFileName;
    string descriptionFileName;
    createTestSuiteFileNameParts(caseNumber, "-sbml-" + version + ".xml", modelFilePath, modelFileName, settingsFileName, descriptionFileName);

    //The following will load and compile and simulate the sbml model in the file
    simulation.SetCaseNumber(caseNumber);
    simulation.SetModelFilePath(modelFilePath);
    simulation.SetModelFileName(modelFileName);
    simulation.ReCompileIfDllExists(true);
    simulation.CopyFilesToOutputFolder();

    rr.setConservedMoietyAnalysis(false);

    if (!simulation.LoadSBMLFromFile())
    {
        Log(Logger::LOG_ERROR) << "Failed loading SBML model";
        throw("Failed loading SBML model");
    }
    //Then read settings file if it exists..
    string settingsOveride("");
    if (!simulation.LoadSettings(settingsOveride))
    {
        Log(Logger::LOG_ERROR) << "Failed loading SBML model settings";
        throw("Failed loading SBML model settings");
    }


    if (!isFBCTest(modelFilePath + "/" + descriptionFileName)) {
        //Only try simulating non-FBC tests.
        if (!simulation.Simulate())
        {
            Log(Logger::LOG_ERROR) << "Failed running simulation";
            throw("Failed running simulation");
        }
    }

}
