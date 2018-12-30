#pragma once

namespace local_ru
{
	const char * const DeleteFileInfo 		= "Вы уверены что хотите удалить этот файл?";
	const char * const DeleteFileHeader		= "Удалить файл?";
	static std::string header[5]			= { (char*)"Имя", (char*)"Дата изменения", (char*)"Тип", (char*)"Размер" , (char*)"Дата создания" };
	static LPCTSTR window_name				= "Прооводник";
	const char * const Folder				= "Папка с файлами";
	const char * const BTN_CREATE			= "Создать";
	const char * const BTN_DELETE			= "Удалить";
	const char * const BTN_COPY				= "Копировать";
	const char * const BTN_PASTE			= "Вставить";
	const char * const BTN_MOVE				= "Вырезать";
	const char * const ErrorCNewFile		= "Ошибка при создании нового файла";
	const char * const ErrorCNewDir			= "Ошибка при создании новой папки";
	const char * const Error				= "Ошибка!";
	const char * const ErrorDelete			= "Не удалось удалить объект!";
	const char * const MenuOpen				= "Открыть";
	const char * const MenuCut				= "Вырезать";
	const char * const MenuCopy				= "Копировать";
	const char * const MenuDelete			= "Удалить";
	const char * const MenuPaste			= "Вставить";
	const char * const MenuRename			= "Переименовать";
	const char * const MenuCreate			= "Создать";
	const char * const MenuCreateFile		= "Папку";
	const char * const MenuCreateTxt		= "Текстовый документ";
	const char * const MenuInfo				= "Свойства";
	const char * const MenuLink				= "Создать ярлык";
	const char * const DefaultTextFile		= "Новый текстовый файл";
}