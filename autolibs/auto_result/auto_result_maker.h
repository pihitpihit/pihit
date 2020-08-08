#ifndef __ResultMaker_Base__
#define __ResultMaker_Base__

#include <exception>
#include <vector>

char* demangle( const char* mangledName );

#define AUTO_POSITION_IN    \
	const char* file,       \
	const int   line,       \
	const char* func

#define AUTO_POSITION                       \
	const char* file = __builtin_FILE(),    \
	const int   line = __builtin_LINE(),    \
	const char* func = __builtin_FUNCTION()

#define AUTO_POSITION_PASS	file, line, func

class exception_t: public std::exception
{
	public:
		virtual const char* name() const;
		virtual const char* desc() const;
		virtual const char* what() const noexcept;
};

class result_record;

/*************************/
/* Result-Exception Code */
/*************************/
class result_code: public exception_t
{
	public:
		const char* name() const override;
		const char* desc() const override;
		const char* what() const noexcept override;
		result_record operator()( const char* msg = nullptr, AUTO_POSITION ) const;

	friend class result_t;

	protected:
		result_code();
		~result_code();
	protected:
		virtual uint32_t getId() const;
		virtual const char* getName() const;
		virtual const char* getDesc() const;
	protected:
		const result_code* result_;
};

/*************************/
/* Result-Exception Type */
/*************************/
class result_t: public exception_t
{
	protected:
		result_t();
	public:
		result_t( const result_t& result, AUTO_POSITION );
		result_t( const result_code& result, AUTO_POSITION );
		result_t( const result_record& result, AUTO_POSITION );
		result_t( const int error );
		~result_t();
	public:
		uint32_t id() const;
		operator int() const;
		result_t& operator=( const result_t& result );
		result_t& operator=( const result_code& result );
		result_t& operator=( const result_record& result );
		result_t& operator=( const int error );
		const char* name() const override;
		const char* desc() const override;
		const char* what() const noexcept override;
	public:
		void backtrace( AUTO_POSITION );
	private:
		void initLog();
		bool isType( uint32_t type );
	protected:
		const result_code* result_;
		std::vector<result_record>* logs_;
		uint32_t type_;
	private:
		const char* file_;
		const int line_;
		const char* func_;
};

/**************************/
/* Error-Result Converter */
/**************************/
result_t ErrorToResult( const int error );

/***********************************/
/* Result-Exception Record Manager */
/***********************************/
class result_record: public exception_t
{
	friend class result_code;
	friend class result_t;
	private:
		result_record( const result_code& result, const char* msg = nullptr, AUTO_POSITION );
	public:
		~result_record();
	public:
		const char* name() const override;
		const char* desc() const override;
		const char* what() const noexcept override;
	private:
		result_record& addType( uint32_t type );
		result_record& subType( uint32_t type );
	public:
		result_record& onAssign();
		result_record& onFinal();
		result_record& byThrow();
	private:
		const char* msg_;
		const char* file_;
		const int line_;
		const char* func_;
		const result_code* result_;
		uint32_t typeOn_;
		uint32_t typeOff_;
};

/************************************************/
/* Result-Exception Hybrid Class Auto Generator */
/************************************************/
#define ResultClassMakerBase( _name, _desc )\
	constexpr char _name##__name[] = #_name;\
	constexpr char _name##__desc[] = _desc;\
	class _name##_t: public result_code\
	{\
	public:\
		_name##_t(){}\
		~_name##_t(){}\
	public:\
		constexpr uint32_t id() const { return (uint32_t)ResultCode::_name; }\
		constexpr operator int() const { return (uint32_t)ResultCode::_name; }\
		uint32_t getId() const override { return id(); }\
		const char* getName() const override { return _name##__name; }\
		const char* getDesc() const override { return _name##__desc; }\
	};\
	const _name##_t _name;

/********************************************************/
/* Macro Function Overloading (Optional Argument Trick) */
/********************************************************/
#define ResultClassMaker1( _name )\
	ResultClassMakerBase( _name, #_name )
#define ResultClassMaker2( _name, _desc )\
	ResultClassMakerBase( _name, _desc )
#define KiMacroGetArg1( _arg1, ... )\
	_arg1
#define KiMacroGetArg3( _arg1, _arg2, _arg3, ... )\
	_arg3
#define ResultClassMaker( ... )\
	KiMacroGetArg3( __VA_ARGS__, ResultClassMaker2, ResultClassMaker1 )( __VA_ARGS__ )

#endif//__ResultMaker_Base__

/**********************************************/
/* Result Code-Class-Exception Auto Generator */
/**********************************************/
#undef ResultMakerStart
#undef ResultMaker
#undef ResultMakerEnd

#if defined(__ResultMaker_Enumeration__)
#	ifndef __ResultMaker_Enumeration_Done__
#	define __ResultMaker_Enumeration_Done__
#		define ResultMakerStart()       enum class ResultCode {
#		define ResultMaker( ... )           KiMacroGetArg1( __VA_ARGS__ ),
#		define ResultMakerEnd()         };
#	endif//__ResultMaker_Enumeration_Done__
#elif defined(__ResultMaker_Class__)
#	ifndef __ResultMaker_Class_Done__
#	define __ResultMaker_Class_Done__
#		define ResultMakerStart()       namespace Result {
#		define ResultMaker( ... )           ResultClassMaker( __VA_ARGS__ )
#		define ResultMakerEnd()         };
#	endif//__ResultMaker_Class_Done__
#else
#	ifndef __ResultMaker_Dummy__
#	define __ResultMaker_Dummy__
#		define ResultMakerStart()
#		define ResultMaker( ... )
#		define ResultMakerEnd()
#	endif//__ResultMaker_Dummy__
#endif

