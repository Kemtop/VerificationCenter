using ReactiveUI;
using System.Collections.ObjectModel;
using System.Reactive;
using System.Reactive.Linq;
using System.Threading.Tasks;
using System.Windows.Input;
using Avalonia.Controls;
using Avalonia.Media.Imaging;
using MessageBox.Avalonia;
using MessageBox.Avalonia.DTO;
using MessageBox.Avalonia.Models;
using MessageBoxAvaloniaEnums = MessageBox.Avalonia.Enums;

namespace VanishBox.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        public ObservableCollection<string> FilesPaths { get; private set; } = new();

        public ReactiveCommand<object, Unit> SelectFilesCommand { get; }

        public ReactiveCommand<object, Unit> ResetSettingsCommand { get; }

        private string[] _selectedFiles { get; set; }

        public MainWindowViewModel()
        {
            SelectFilesCommand = ReactiveCommand.CreateFromTask<object>(async (sender) =>
            {
                await SelectFilesAsync(sender);
            });

            ResetSettingsCommand = ReactiveCommand.CreateFromTask<object>(async (sender) =>
            {
                await ResetSettings(sender);
            });
        }
        
        /// <summary>
        /// Выбрать файлы которые требуется зашифровать/расшифровать.
        /// </summary>
        /// <param name="sender"></param>
        /// <returns></returns>
        private async Task SelectFilesAsync(object sender)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = "Выбрать файлы";
            dialog.AllowMultiple = true;
            dialog.Filters.Add(new FileDialogFilter() { Name = "All", Extensions = { "*" } });
            _selectedFiles = await dialog.ShowAsync(sender as Window);
            
            FilesPaths.Clear();
            foreach (var item in _selectedFiles)
            {
                FilesPaths.Add(item);
            }
        }

        /// <summary>
        /// Сброс настроек программы.
        /// </summary>
        /// <param name="sender"></param>
        /// <returns></returns>
        private async Task ResetSettings(object sender)
        {
            var buttonNo = new ButtonDefinition { Name = "Нет", IsDefault = true };

            var messageBoxCustomWindow = MessageBoxManager
                .GetMessageBoxCustomWindow(new MessageBoxCustomParamsWithImage
                {
                    Topmost = true,
                    ContentMessage = "Вы действительно хотите сбросить настройки?",
                    Icon = new Bitmap("./Assets/info.png"),
                    
                    ButtonDefinitions = new[]
                    {
                        new ButtonDefinition { Name = "Да", IsCancel = true },
                        buttonNo
                    },
                    WindowStartupLocation = WindowStartupLocation.CenterOwner,
                    WindowIcon = new WindowIcon("./Assets/info.png"),
                });

            var result = await messageBoxCustomWindow.ShowDialog(sender as Window);

            if(result == buttonNo.Name) return;
            
        }
    }
}