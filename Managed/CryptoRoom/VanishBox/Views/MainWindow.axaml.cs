using Avalonia.ReactiveUI;
using ReactiveUI;
using System.Threading.Tasks;
using VanishBox.ViewModels;

namespace VanishBox.Views
{
    public partial class MainWindow : ReactiveWindow<MainWindowViewModel>
    {
        private readonly IDialogService _dialogService;
        public MainWindow()
        {
            InitializeComponent();
            _dialogService = new DialogService(this);
            this.WhenActivated(d =>
            {
                ViewModel!.ShowKeyParamDialog.RegisterHandler(DoShowKeyParamDialogAsync);
                ViewModel!.ResetSettingsDialog.RegisterHandler(_dialogService.ShowConfirmDialogAsync);
                ViewModel!.SelectFilesDialog.RegisterHandler(_dialogService.SelectFilesAsync);
            });
        }
       
        /// <summary>
        /// ���� ����� ������ � ���� ���������� ���������� �����.
        /// </summary>
        /// <param name="interaction"></param>
        /// <returns></returns>
        private async Task DoShowKeyParamDialogAsync(InteractionContext<PasswordInputWindowViewModel, UserKeyViewModel?> interaction)
        {
            var dialog = new PasswordInputWindow();
            dialog.DataContext = interaction.Input;

            var result = await dialog.ShowDialog<UserKeyViewModel?>(this);
            interaction.SetOutput(result);
        }
    }
}