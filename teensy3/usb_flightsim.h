#ifndef USBflightsim_h_
#define USBflightsim_h_

#if defined(USB_FLIGHTSIM) && defined(__cplusplus)

#include <inttypes.h>
#include "elapsedMillis.h"

class FlightSimClass;
class FlightSimCommand;
class FlightSimInteger;

class _XpRefStr_;
#define XPlaneRef(str) ((const _XpRefStr_ *)(str))

class FlightSimClass
{
public:
	FlightSimClass();
	static void update(void);
	static bool isEnabled(void);
	static unsigned long getFrameCount(void) { return frameCount; }
private:
	static uint8_t request_id_messages;
	static uint8_t enabled;
	static elapsedMillis enableTimeout;
	static unsigned long frameCount;
	static void enable(void) { enabled = 1; enableTimeout = 0; }
	static void disable(void) { enabled = 0; }
	static void xmit(const void *p1, uint8_t n1, const void *p2, uint8_t n2);
	friend class FlightSimCommand;
	friend class FlightSimInteger;
	friend class FlightSimFloat;
};


class FlightSimCommand
{
public:
	FlightSimCommand();
	void assign(const _XpRefStr_ *s) { name = s; if (FlightSimClass::enabled) identify(); }
	FlightSimCommand & operator = (const _XpRefStr_ *s) { assign(s); return *this; }
	void begin(void) { sendcmd(4); }
	void end(void) { sendcmd(5); }
	FlightSimCommand & operator = (int n) { sendcmd((n) ? 4 : 5); return *this; }
	void once(void) { sendcmd(6); }
	void identify(void);
private:
	unsigned int id;
	const _XpRefStr_ *name;
	void sendcmd(uint8_t n);
	FlightSimCommand *next;
	static FlightSimCommand *first;
	static FlightSimCommand *last;
	friend class FlightSimClass;
};


class FlightSimInteger
{
public:
	FlightSimInteger();
	void assign(const _XpRefStr_ *s) { name = s; if (FlightSimClass::enabled) identify(); }
	FlightSimInteger & operator = (const _XpRefStr_ *s) { assign(s); return *this; }
	void write(long val);
	FlightSimInteger & operator = (char n) { write((long)n); return *this; }
	FlightSimInteger & operator = (int n) { write((long)n); return *this; }
	FlightSimInteger & operator = (long n) { write(n); return *this; }
	FlightSimInteger & operator = (unsigned char n) { write((long)n); return *this; }
	FlightSimInteger & operator = (unsigned int n) { write((long)n); return *this; }
	FlightSimInteger & operator = (unsigned long n) { write((long)n); return *this; }
	FlightSimInteger & operator = (float n) { write((long)n); return *this; }
	FlightSimInteger & operator = (double n) { write((long)n); return *this; }
	long read(void) const { return value; }
	operator long () const { return value; }
	void identify(void);
	void update(long val);
	static FlightSimInteger * find(unsigned int n);
	void onChange(void (*fptr)(long)) { change_callback = fptr; }
	// TODO: math operators....  + - * / % ++ --
private:
	unsigned int id;
	const _XpRefStr_ *name;
	long value;
	void (*change_callback)(long);
	FlightSimInteger *next;
	static FlightSimInteger *first;
	static FlightSimInteger *last;
	friend class FlightSimClass;
};


class FlightSimFloat
{
public:
	FlightSimFloat();
	void assign(const _XpRefStr_ *s) { name = s; if (FlightSimClass::enabled) identify(); }
	FlightSimFloat & operator = (const _XpRefStr_ *s) { assign(s); return *this; }
	void write(float val);
	FlightSimFloat & operator = (char n) { write((float)n); return *this; }
	FlightSimFloat & operator = (int n) { write((float)n); return *this; }
	FlightSimFloat & operator = (long n) { write((float)n); return *this; }
	FlightSimFloat & operator = (unsigned char n) { write((float)n); return *this; }
	FlightSimFloat & operator = (unsigned int n) { write((float)n); return *this; }
	FlightSimFloat & operator = (unsigned long n) { write((float)n); return *this; }
	FlightSimFloat & operator = (float n) { write(n); return *this; }
	FlightSimFloat & operator = (double n) { write((float)n); return *this; }
	float read(void) const { return value; }
	operator float () const { return value; }
	void identify(void);
	void update(float val);
	static FlightSimFloat * find(unsigned int n);
	void onChange(void (*fptr)(float)) { change_callback = fptr; }
	// TODO: math operators....  + - * / % ++ --
private:
	unsigned int id;
	const _XpRefStr_ *name;
	float value;
	void (*change_callback)(float);
	FlightSimFloat *next;
	static FlightSimFloat *first;
	static FlightSimFloat *last;
	friend class FlightSimClass;
};


class FlightSimElapsedFrames
{
private:
	unsigned long count;
public:
	FlightSimElapsedFrames(void) { count = FlightSimClass::getFrameCount(); }
	FlightSimElapsedFrames(unsigned long val) { count = FlightSimClass::getFrameCount() - val; }
	FlightSimElapsedFrames(const FlightSimElapsedFrames &orig) { count = orig.count; }
	operator unsigned long () const { return FlightSimClass::getFrameCount() - count; }
	FlightSimElapsedFrames & operator = (const FlightSimElapsedFrames &rhs) { count = rhs.count; return *this; }
	FlightSimElapsedFrames & operator = (unsigned long val) { count = FlightSimClass::getFrameCount() - val; return *this; }
	FlightSimElapsedFrames & operator -= (unsigned long val)    { count += val; return *this; }
	FlightSimElapsedFrames & operator += (unsigned long val)    { count -= val; return *this; }
	FlightSimElapsedFrames operator - (int val) const           { FlightSimElapsedFrames r(*this); r.count += val; return r; }
	FlightSimElapsedFrames operator - (unsigned int val) const  { FlightSimElapsedFrames r(*this); r.count += val; return r; }
	FlightSimElapsedFrames operator - (long val) const          { FlightSimElapsedFrames r(*this); r.count += val; return r; }
	FlightSimElapsedFrames operator - (unsigned long val) const { FlightSimElapsedFrames r(*this); r.count += val; return r; }
	FlightSimElapsedFrames operator + (int val) const           { FlightSimElapsedFrames r(*this); r.count -= val; return r; }
	FlightSimElapsedFrames operator + (unsigned int val) const  { FlightSimElapsedFrames r(*this); r.count -= val; return r; }
	FlightSimElapsedFrames operator + (long val) const          { FlightSimElapsedFrames r(*this); r.count -= val; return r; }
	FlightSimElapsedFrames operator + (unsigned long val) const { FlightSimElapsedFrames r(*this); r.count -= val; return r; }
};


extern FlightSimClass FlightSim;


#endif
#endif
