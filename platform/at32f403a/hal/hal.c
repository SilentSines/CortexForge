#include "hal.h"

static void system_clock_config(void)
{
	/* reset crm */
	crm_reset();
	/* enable lick */
	crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);
	/* wait till lick is ready */
	while (crm_flag_get(CRM_LICK_STABLE_FLAG) != SET);
	/* enable hext */
	crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
	/* wait till hext is ready */
	while (crm_hext_stable_wait() == ERROR);
	/* enable hick */
	crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);
	/* wait till hick is ready */
	while (crm_flag_get(CRM_HICK_STABLE_FLAG) != SET);
	/* config pll clock resource */
	crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_50, CRM_PLL_OUTPUT_RANGE_GT72MHZ);
	/* config hext division */
	crm_hext_clock_div_set(CRM_HEXT_DIV_2);
	/* enable pll */
	crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
	/* wait till pll is ready */
	while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET);
	/* config ahbclk */
	crm_ahb_div_set(CRM_AHB_DIV_1);
	/* config apb2clk, the maximum frequency of APB2 clock is 120 MHz  */
	crm_apb2_div_set(CRM_APB2_DIV_2);
	/* config apb1clk, the maximum frequency of APB1 clock is 120 MHz  */
	crm_apb1_div_set(CRM_APB1_DIV_2);
	/* enable auto step mode */
	crm_auto_step_mode_enable(TRUE);
	/* select pll as system clock source */
	crm_sysclk_switch(CRM_SCLK_PLL);
	/* wait till pll is used as system clock source */
	while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL);
	/* disable auto step mode */
	crm_auto_step_mode_enable(FALSE);
	/* update system_core_clock global variable */
	system_core_clock_update();

	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	// crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	// crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);

	crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
	gpio_pin_remap_config(SWJTAG_GMUX_010, TRUE);
}

static void nvic_config(void)
{
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

	NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
}

void hal_init(void)
{
	__enable_irq();
	system_clock_config();
	nvic_config();
	systick_init();
}

void hal_error_handler(void)
{
	__disable_irq();
	while (1)
	{
		
	}
}