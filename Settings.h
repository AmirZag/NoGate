#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGlobal>
#include <QByteArray>

namespace Afc
{
struct Settings
{
  // General.
  static constexpr quint16  MAIN_SOFTWARE_VERSION = 0x0205;
  static constexpr quint16  AUX_SOFTWARE_VERSION  = 0x0000;
  static constexpr quint8   COMPANY_ID            = 3;
  static constexpr quint8   CONTRACT_ID           = 1;
  static constexpr quint8   EQUIPMENT_ID          = 0;
  static constexpr quint8   EQUIPMENT_SUB_TYPE    = 2;

  // User.
  static constexpr auto USER_NAME = "maintenance";
  static constexpr auto USER_PASS = "110114";

  // Gate.
  static constexpr auto  GATE_CONTROLLER_PORT_NAME = "GateControllerPortName";

  // Reader.
  static constexpr auto  CARD_READER_PORT_NAME = "CardReaderPortName";
  static constexpr auto  CITYPAY_SAM_SLOT      = "CityPaySamSlot";
  static constexpr auto  CITYPAY_SAM_KEY       = "CityPaySamKey";

  // Scu.
  static constexpr auto  SCU_BASE_PARAM_FILE_NAME = "FAMILLE";
  static constexpr auto  SCU_INITIATE_FILE_NAME   = "INITIATE";
  static constexpr auto  SCU_ACTS_DIR             = "/home/maintenance/acts";
  static constexpr auto  SCU_PARAMS_DIR           = "/home/maintenance/parameters";
  static constexpr auto  SCU_PARAM_EXT            = ".BIN";
  static constexpr auto  SCU_ACTS_EXT             = ".dat";

  // Ticket.
  static constexpr auto  TICKET_TYPE_CITYPAY = 3;
  static constexpr auto  TICKET_TYPE_QR      = 242;

  // QR.
  static constexpr auto  QR_DATA_DIR  = "/home/maintenance/qr";
  static constexpr auto  QR_SERVER_IP = "QrServerIp";

  // Styles.
  static constexpr auto  STYLE_WHITE_BLACK =
    "font-family: \"Sahel Black FD\"; font-size: 45pt; background-color: transparent; color: rgb(0, 0, 0)";
  static constexpr auto  STYLE_GREEN_WHITE =
    "font-family: \"Sahel Black FD\"; font-size: 45pt; background-color: rgb(0, 155, 50); color: rgb(255, 255, 255)";
  static constexpr auto  STYLE_YELLOW_WHITE =
    "font-family: \"Sahel Black FD\"; font-size: 45pt; background-color: rgb(200, 130, 0); color: rgb(255, 255, 255)";
  static constexpr auto  STYLE_RED_WHITE =
    "font-family: \"Sahel Black FD\"; font-size: 45pt; background-color: rgb(205, 0, 0); color: rgb(255, 255, 255)";
  static constexpr auto  STYLE_WHITE_BLACK_S =
    "font-family: \"Sahel Black FD\"; font-size: 45pt; background-color: transparent; color: rgba(90, 90, 90, 100)";
  static constexpr auto  STYLE_GREEN_WHITE_S =
    "font-family: \"Sahel Black FD\"; font-size: 45pt; background-color: rgb(0, 155, 50); color: rgba(120, 120, 120, 100)";
  static constexpr auto  STYLE_YELLOW_WHITE_S =
    "font-family: \"Sahel Black FD\"; font-size: 45pt; background-color: rgb(200, 130, 0); color: rgba(120, 120, 120, 100)";
  static constexpr auto  STYLE_RED_WHITE_S =
    "font-family: \"Sahel Black FD\"; font-size: 45pt; background-color: rgb(205, 0, 0); color: rgba(0, 0, 0, 100)";
  static constexpr auto  STYLE2_BLACK     = "font-family: \"Liberation Serif Regular\"; font-size: 18pt; color: rgb(0, 0, 0)";
  static constexpr auto  STYLE2_WHITE     = "font-family: \"Liberation Serif Regular\"; font-size: 18pt; color: rgb(255, 255, 255)";
  static constexpr auto  STYLE_LOGO_BLACK = "font-family: \"Sahel Black FD\"; font-size: 18pt; color: rgb(0, 0, 0)";
  static constexpr auto  STYLE_LOGO_WHITE = "font-family: \"Sahel Black FD\"; font-size: 18pt; color: rgb(255, 255, 255)";
  static constexpr auto  STYLE_TIME_BLACK =
    "image: url(:/images/banner.png); image-position:bottom; font-family: \"Sahel Black FD\"; font-size: 20pt; color: rgb(0, 0, 0)";
  static constexpr auto  STYLE_TIME_WHITE =
    "image: url(:/images/banner.png); image-position:bottom; font-family: \"Sahel Black FD\"; font-size: 20pt; color: rgb(255, 255, 255)";
  static constexpr auto  STYLE_SETTING_BLACK =
    "background: transparent; font-family: \"Sahel Black FD\"; font-size: 22pt; color: rgb(0, 0, 0); border: 0; outline: 0;";
  static constexpr auto  STYLE_SETTING_WHITE =
    "background: transparent; font-family: \"Sahel Black FD\"; font-size: 22pt; color: rgb(255, 255, 255); border: 0; outline: 0;";
};
}

#endif // SETTINGS_H
