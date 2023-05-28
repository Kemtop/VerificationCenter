using ReactiveUI;
using System.Threading.Tasks;

namespace VanishBox
{
    public interface IDialogService
    {
        /// <summary>
        /// Выбрать каталог.
        /// </summary>
        /// <param name="interaction"></param>
        /// <returns></returns>
        Task SelectDirAsync(InteractionContext<object, string?> interaction);

        /// <summary>
        /// Выбрать файлы из каталога.
        /// </summary>
        /// <param name="interaction"></param>
        /// <returns></returns>
        Task SelectFilesAsync(InteractionContext<object, string[]?> interaction);

        /// <summary>
        /// Диалог подтверждения с кнопками «Да», «Нет».
        /// </summary>
        /// <param name="interaction"></param>
        /// <returns></returns>

        Task ShowConfirmDialogAsync(InteractionContext<string, bool> interaction);

        /// <summary>
        /// Окно с текстом ошибки.
        /// </summary>
        /// <param name="interaction"></param>
        /// <returns></returns>
        Task ShowErrorDialogAsync(InteractionContext<string, string?> interaction);
    }
}
