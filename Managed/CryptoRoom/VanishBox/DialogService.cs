using Avalonia.Controls;
using System.Threading.Tasks;
using ReactiveUI;
using MessageBox.Avalonia.DTO;
using MessageBox.Avalonia.Models;
using MessageBoxAvaloniaEnums = MessageBox.Avalonia.Enums;

namespace VanishBox
{
    public class DialogService : IDialogService
    {
        private readonly Window _parentWindow;

        public DialogService(Window parentWindow)
        {
            _parentWindow = parentWindow;
        }
        
        public async Task SelectFilesAsync(InteractionContext<object, string[]?> interaction)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = "Выбрать файлы";
            dialog.AllowMultiple = true;
            dialog.Filters.Add(new FileDialogFilter() { Name = "All", Extensions = { "*" } });
            var result = await dialog.ShowAsync(_parentWindow);
            interaction.SetOutput(result);
        }

        /// <summary>
        /// Выбрать каталог.
        /// </summary>
        /// <param name="interaction"></param>
        /// <returns></returns>
        public async Task SelectDirAsync(InteractionContext<object, string?> interaction)
        {
            var dialog = new OpenFolderDialog();
            var result = await dialog.ShowAsync(_parentWindow);
            interaction.SetOutput(result);
        }

        /// <summary>
        /// Диалог подтверждения с кнопками «Да», «Нет».
        /// </summary>
        /// <param name="interaction"></param>
        /// <returns></returns>
        public async Task ShowConfirmDialogAsync(InteractionContext<string, bool> interaction)
        {

            var buttonNo = new ButtonDefinition { Name = "Нет", IsDefault = true };

            var messageBoxCustomWindow = MessageBox.Avalonia.MessageBoxManager.GetMessageBoxCustomWindow(
                new MessageBoxCustomParams
                {
                    ContentTitle = "Ошибка",
                    ContentMessage = interaction.Input,
                    FontFamily = "Microsoft YaHei,Simsun",
                    Icon = MessageBoxAvaloniaEnums.Icon.Error,
                    WindowIcon = new WindowIcon("./Assets/app.ico"),
                    ButtonDefinitions = new[]
                    {
                        new ButtonDefinition { Name = "Да", IsDefault = false },
                        buttonNo
                    },
                    WindowStartupLocation = WindowStartupLocation.CenterOwner
                });
            var result = await messageBoxCustomWindow.ShowDialog(_parentWindow);
            
            bool action = result != buttonNo.Name;

            interaction.SetOutput(action);
        }

        /// <summary>
        /// Окно с текстом ошибки.
        /// </summary>
        /// <param name="interaction"></param>
        /// <returns></returns>
        public async Task ShowErrorDialogAsync(InteractionContext<string, string?> interaction)
        {
            var messageBoxCustomWindow = MessageBox.Avalonia.MessageBoxManager.GetMessageBoxCustomWindow(
                new MessageBoxCustomParams
                {
                    ContentTitle = "Ошибка",
                    ContentMessage = interaction.Input,
                    FontFamily = "Microsoft YaHei,Simsun",
                    Icon = MessageBoxAvaloniaEnums.Icon.Error,
                    WindowIcon = new WindowIcon("./Assets/app.ico"),
                    ButtonDefinitions = new[]
                        { new ButtonDefinition { Name = "Ок", IsDefault = true }, },
                    WindowStartupLocation = WindowStartupLocation.CenterOwner
                });
            await messageBoxCustomWindow.ShowDialog(_parentWindow);

            interaction.SetOutput(null);
        }
    }
}
