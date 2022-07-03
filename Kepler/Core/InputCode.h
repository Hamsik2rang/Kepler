#pragma once

#include <stdint.h>

namespace kepler {

	using KeyCode = uint16_t;
	using MouseCode = uint16_t;

	// Key Input Code
	namespace key {
		// ref from WinAPI Virtual Key Code
		enum : KeyCode
		{
			Cancel			= 3,
			
			Backspace		= 8,
			Tab				= 9,
			
			Clear			= 12,
			Return			= 13,
			Enter			= 13,

			Shift			= 16,
			Ctrl			= 17,
			Alt				= 18,
			Pause			= 19,
			Caps			= 20,
			
			Escape			= 27,
			
			Space			= 32,
			PageUp			= 33,
			PageDown		= 34,
			End				= 35,
			Home			= 36,

			// Arrow
			Left			= 37,
			Up				= 38,
			Right			= 39,
			Down			= 40,

			Print			= 42,
			
			Screenshot		= 44,
			Insert			= 45,
			Delete			= 46,

			Num0			= 48,
			Num1			= 49,
			Num2			= 50,
			Num3			= 51,
			Num4			= 52, 
			Num5			= 53, 
			Num6			= 54, 
			Num7			= 55,
			Num8			= 56,
			Num9			= 57,



			A				= 65,
			B				= 66,
			C				= 67,
			D				= 68,
			E				= 69,
			F				= 70,
			G				= 71,
			H				= 72,
			I				= 73,
			J				= 74,
			K				= 75,
			L				= 76,
			M				= 77,
			N				= 78,
			O				= 79,
			P				= 80,
			Q				= 81,
			R				= 82,
			S				= 83,
			T				= 84,
			U				= 85,
			V				= 86,
			W				= 87,
			X				= 88,
			Y				= 89,
			Z				= 90,

			LeftWindow		= 91,
			RightWindow		= 92,

			Numpad0			= 96,
			Numpad1			= 97,
			Numpad2			= 98,
			Numpad3			= 99,
			Numpad4			= 100,
			Numpad5			= 101,
			Numpad6			= 102,
			Numpad7			= 103,
			Numpad8			= 104,
			Numpad9			= 105,

			F1				= 112,
			F2				= 113,
			F3				= 114,
			F4				= 115,
			F5				= 116,
			F6				= 117,
			F7				= 118,
			F8				= 119,
			F9				= 120,
			F10				= 121,
			F11				= 122,
			F12				= 123,
			F13				= 124,
			F14				= 125,
			F15				= 126,
			F16				= 127,
			F17				= 128,
			F18				= 129,
			F19				= 130,
			F20				= 131,
			F21				= 132,
			F22				= 133,
			F23				= 134,
			F24				= 135,

			NumLock			= 144,
			ScrollLock		= 145,

			Semicolon		= 186, // ;
			Plus			= 187, // +
			Comma			= 188, // ,
			Munus			= 189, // -
			Period			= 190, // .
			Slash			= 191, // /
			GraveAccent		= 192, // `

			OpenBracket		= 219, // [
			BackShash		= 220, // |
			CloseBracket	= 221, // ]
			Apostrophe		= 222, // '"
			Extra			= 223, // It can vary by keyboard.
		};
	}

	namespace mouse
	{
		// Mouse Input Code
		// ref from WinAPI Virtual Key Code
		enum : MouseCode
		{
			Button0 = 1,
			Button1 = 2,
			Button2 = 4,
			Button3 = 5,
			Button4 = 6,

			Left = 1,
			Right = 2,
			Middle = 3,
		};
	}
}