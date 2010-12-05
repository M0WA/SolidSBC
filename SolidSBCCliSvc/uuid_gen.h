#pragma once

class CUuidGen
{
	public:
		CUuidGen(void);
		~CUuidGen(void);
		static char* CreateUUID(void);

	private:
		static unsigned int rand32(void);
};