#ifndef LIBGRAMAS_EXPORT_H
#define LIBGRAMAS_EXPORT_H

#ifdef WIN32
#	define	GR_EXPORT	__declspec(dllexport)
#else
#	define	GR_EXPORT
#endif

#ifdef __cplusplus
#	define	GR_CDECL_BEGIN	export "C" {
#	define	GR_CDECL_END	}
#else
#	define	GR_CDECL_BEGIN
#	define	GR_CDECL_END
#endif

#endif /* LIBGRAMAS_EXPORT_H */