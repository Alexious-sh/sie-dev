
#if !defined(_PMB8876_REG_H_)
#define _PMB8876_REG_H_

#define	PMB8876_VIRTUAL_ADDRESS(x)	x

#define PMB8876_REG(_register_)		*((volatile unsigned int *)(PMB8876_VIRTUAL_ADDRESS(_register_)))
#define PMB8876_REG_SET_BIT(reg, bit)	PMB8876_REG(reg) |= bit
#define PMB8876_REG_CLEAR_BIT(reg, bit)	PMB8876_REG(reg) = (PMB8876_REG(reg) & ~(bit))
#define PMB8876_REG_SET_MASK(reg, mask)	PMB8876_REG(reg) = mask
#define PMB8876_REG_BYTE(_register_)	*((volatile unsigned char *)(PMB8876_VIRTUAL_ADDRESS(_register_)))

#endif	/* !defined(_PMB8876_REG_H_) */
