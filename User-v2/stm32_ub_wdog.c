

void IWDG_Init(uint8_t prer, uint8_t rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* 0x5555ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����*/
	IWDG_SetPrescaler(prer);    /*����IWDGԤ��Ƶֵ:����IWDGԤ��Ƶֵ*/
	IWDG_SetReload(rlr);     /*����IWDG��װ��ֵ*/
	IWDG_ReloadCounter();    /*����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������*/
	IWDG_Enable();        /*0xcccc, ʹ��IWDG*/
}

/**
 * ι�������Ź�
 */
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();    /*0xaaaa reload*/
}
