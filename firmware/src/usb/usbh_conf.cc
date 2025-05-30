/**
 ******************************************************************************
 * @file           : usbh_conf.c
 * @version        : v1.0_Cube
 * @brief          : This file implements the board support package for the USB host library
 * @author         : STM, with modifications of the H7 example for stm32mp1-baremetal by Dan Green
 ******************************************************************************
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

#include "usbh_core.h"
#include "usbh_msc.h"

/*******************************************************************************
					   HCD BSP Routines
*******************************************************************************/
/**
 * @brief  Initializes the HCD MSP.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_MspInit(HCD_HandleTypeDef *hpcd) {
	if (hpcd->Instance == USB_OTG_HS) {
		// Reset peripheral so we can switch host/device
		__HAL_RCC_USBO_FORCE_RESET();
		__HAL_RCC_USBO_RELEASE_RESET();

		__HAL_RCC_USBO_CLK_ENABLE();
		__HAL_RCC_USBPHY_CLK_ENABLE();

		// SWITHOST 0: Select OTG controller for 2nd PHY port
		USBPHYC->MISC = (USBPHYC->MISC & ~(USBPHYC_MISC_SWITHOST_Msk | USBPHYC_MISC_PPCKDIS_Msk)) |
						(0 << USBPHYC_MISC_SWITHOST_Pos) | (0 << USBPHYC_MISC_PPCKDIS_Pos);

		HAL_PWREx_EnableUSBVoltageDetector();
	}
}

void HAL_HCD_MspDeInit(HCD_HandleTypeDef *hpcd) {
	if (hpcd->Instance == USB_OTG_HS) {
		__HAL_RCC_USBO_CLK_DISABLE();
		__HAL_RCC_USBPHY_CLK_DISABLE();
	}
}

/*******************************************************************************
					   LL Driver Callbacks (HCD -> USB Host Library)
*******************************************************************************/

/**
 * @brief  SOF callback.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd) {
	USBH_LL_IncTimer((USBH_HandleTypeDef *)hhcd->pData);
}

/**
 * @brief  Connect callback.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd) {
	USBH_LL_Connect((USBH_HandleTypeDef *)hhcd->pData);
}

/**
 * @brief  Disconnect callback.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd) {
	USBH_LL_Disconnect((USBH_HandleTypeDef *)hhcd->pData);
}

/**
 * @brief  Port Port Enabled callback.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *hhcd) {
	USBH_LL_PortEnabled((USBH_HandleTypeDef *)hhcd->pData);
}

/**
 * @brief  Port Port Disabled callback.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *hhcd) {
	USBH_LL_PortDisabled((USBH_HandleTypeDef *)hhcd->pData);
}

/**
 * @brief  Notify URB state change callback.
 * @param  hhcd: HCD handle
 * @param  chnum: Channel number
 * @param  urb_state: URB State
 * @retval None
 */
// void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state) {
// 	/* To be used with OS to sync URB state with the global state machine */
// 	// USBH_LL_NotifyURBChange(hhcd->phost);
// }

/*******************************************************************************
					   LL Driver Interface (USB Host Library --> HCD)
*******************************************************************************/

/**
 * @brief  Initialize the low level portion of the host driver.
 * @param  phost: Host handle
 * @retval USBH status
 */
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost) {
	if (HAL_HCD_Init((HCD_HandleTypeDef *)phost->pData) != HAL_OK) {
		return USBH_FAIL;
	}

	USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame((HCD_HandleTypeDef *)phost->pData));

	return USBH_OK;
}

/**
 * @brief  De-Initializes the Low Level portion of the Host driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost) {
	HAL_HCD_DeInit((HCD_HandleTypeDef *)phost->pData);
	return USBH_OK;
}

/**
 * @brief  Starts the Low Level portion of the Host driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost) {
	HAL_HCD_Start((HCD_HandleTypeDef *)phost->pData);
	return USBH_OK;
}

/**
 * @brief  Stops the Low Level portion of the Host driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost) {
	HAL_HCD_Stop((HCD_HandleTypeDef *)phost->pData);
	return USBH_OK;
}

/**
 * @brief  Returns the USB Host Speed from the Low Level Driver.
 * @param  phost: Host handle
 * @retval USBH Speeds
 */
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost) {
	USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

	switch (HAL_HCD_GetCurrentSpeed((HCD_HandleTypeDef *)phost->pData)) {
		case 0:
			speed = USBH_SPEED_HIGH;
			break;

		case 1:
			speed = USBH_SPEED_FULL;
			break;

		case 2:
			speed = USBH_SPEED_LOW;
			break;

		default:
			speed = USBH_SPEED_FULL;
			break;
	}
	return speed;
}

/**
 * @brief  Resets the Host Port of the Low Level Driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost) {
	HAL_HCD_ResetPort((HCD_HandleTypeDef *)phost->pData);
	return USBH_OK;
}

/**
 * @brief  Returns the last transferred packet size.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @retval Packet Size
 */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe) {
	return HAL_HCD_HC_GetXferCount((HCD_HandleTypeDef *)phost->pData, pipe);
}

/**
 * @brief  Opens a pipe of the Low Level Driver.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @param  epnum: Endpoint Number
 * @param  dev_address: Device USB address
 * @param  speed: Device Speed
 * @param  ep_type: Endpoint Type
 * @param  mps: Endpoint Max Packet Size
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost,
									uint8_t pipe,
									uint8_t epnum,
									uint8_t dev_address,
									uint8_t speed,
									uint8_t ep_type,
									uint16_t mps) {
	HAL_HCD_HC_Init((HCD_HandleTypeDef *)phost->pData, pipe, epnum, dev_address, speed, ep_type, mps);
	return USBH_OK;
}

/**
 * @brief  Closes a pipe of the Low Level Driver.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe) {
	HAL_HCD_HC_Halt((HCD_HandleTypeDef *)phost->pData, pipe);
	return USBH_OK;
}

/**
 * @brief  Submits a new URB to the low level driver.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 *          This parameter can be a value from 1 to 15
 * @param  direction: Channel number
 *          This parameter can be one of these values:
 *           0: Output
 *           1: Input
 * @param  ep_type: Endpoint Type
 *          This parameter can be one of these values:
 *            @arg EP_TYPE_CTRL: Control type
 *            @arg EP_TYPE_ISOC: Isochronous type
 *            @arg EP_TYPE_BULK: Bulk type
 *            @arg EP_TYPE_INTR: Interrupt type
 * @param  token: Endpoint Type
 *          This parameter can be one of these values:
 *            @arg 0: PID_SETUP
 *            @arg 1: PID_DATA
 * @param  pbuff: pointer to URB data
 * @param  length: length of URB data
 * @param  do_ping: activate do ping protocol (for high speed only)
 *          This parameter can be one of these values:
 *           0: do ping inactive
 *           1: do ping active
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost,
									 uint8_t pipe,
									 uint8_t direction,
									 uint8_t ep_type,
									 uint8_t token,
									 uint8_t *pbuff,
									 uint16_t length,
									 uint8_t do_ping) {
	HAL_HCD_HC_SubmitRequest(
		(HCD_HandleTypeDef *)phost->pData, pipe, direction, ep_type, token, pbuff, length, do_ping);
	return USBH_OK;
}

/**
 * @brief  Gets a URB state from the low level driver.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 *          This parameter can be a value from 1 to 15
 * @retval URB state
 *          This parameter can be one of these values:
 *            @arg URB_IDLE
 *            @arg URB_DONE
 *            @arg URB_NOTREADY
 *            @arg URB_NYET
 *            @arg URB_ERROR
 *            @arg URB_STALL
 */
USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe) {
	return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState((HCD_HandleTypeDef *)phost->pData, pipe);
}

/**
 * @brief  USBH_LL_DriverVBUS
 *         Drive VBUS.
 * @param  phost: Host handle
 * @param  state : VBUS state
 *          This parameter can be one of the these values:
 *           0 : VBUS Active
 *           1 : VBUS Inactive
 * @retval Status
 */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state) {
	if (state != FALSE) {
		//
	} else {
		//
	}
	// HAL_Delay(200);
	return USBH_OK;
}

/**
 * @brief  Sets toggle for a pipe.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @param  toggle: toggle (0/1)
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle) {
	auto p_hhcd = (HCD_HandleTypeDef *)phost->pData;
	if (p_hhcd->hc[pipe].ep_is_in) {
		p_hhcd->hc[pipe].toggle_in = toggle;
	} else {
		p_hhcd->hc[pipe].toggle_out = toggle;
	}
	return USBH_OK;
}

/**
 * @brief  Returns the current toggle of a pipe.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @retval toggle (0/1)
 */
uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe) {
	auto p_hhcd = (HCD_HandleTypeDef *)phost->pData;
	uint8_t toggle = 0;

	if (p_hhcd->hc[pipe].ep_is_in) {
		toggle = p_hhcd->hc[pipe].toggle_in;
	} else {
		toggle = p_hhcd->hc[pipe].toggle_out;
	}
	return toggle;
}

/**
 * @brief  Delay routine for the USB Host Library
 * @param  Delay: Delay in ms
 * @retval None
 */
void USBH_Delay(uint32_t Delay) {
	HAL_Delay(Delay);
}

void *msc_malloc(size_t sz) {
	static MSC_HandleTypeDef hmsc;
	if (sz == sizeof(MSC_HandleTypeDef))
		return &hmsc;
	else
		return nullptr;
}

void msc_free(void *) {
}
